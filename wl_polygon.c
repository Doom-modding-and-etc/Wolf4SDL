// WL_POLYGON.C
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_POLYGON)

/* Advances the index by one vertex forward through the vertex list,
   wrapping at the end of the list */
#define INDEX_FORWARD(Index) \
   Index = (Index + 1) % VertexList->Length;

/* Advances the index by one vertex backward through the vertex list,
   wrapping at the start of the list */
#define INDEX_BACKWARD(Index) \
   Index = (Index - 1 + VertexList->Length) % VertexList->Length;

/* Advances the index by one vertex either forward or backward through
   the vertex list, wrapping at either end of the list */
#define INDEX_MOVE(Index,Direction) \
   if (Direction > 0) \
      Index = (Index + 1) % VertexList->Length; \
   else \
      Index = (Index - 1 + VertexList->Length) % VertexList->Length;

extern VBuf_t vbuf;

static void DrawHorizontalLineList(struct HLineList * HLineListPtr,
      int Color, unsigned char Blend)
{
    struct HLine *HLinePtr;
    int Y, X;

    /* Point to the XStart/XEnd descriptor for the first (top)
        horizontal line */
    HLinePtr = HLineListPtr->HLinePtr;
    /* Draw each horizontal line in turn, starting with the top one and
        advancing one line each time */
    for (Y = HLineListPtr->YStart; Y < (HLineListPtr->YStart +
            HLineListPtr->Length); Y++, HLinePtr++) {
        if (Y < 0 || Y >= screen.h) {
            continue;
        }
        if (HLinePtr->XStart < 0) {
            HLinePtr->XStart = 0;
        }
        if (HLinePtr->XEnd >= screen.w) {
            HLinePtr->XEnd = screen.w - 1;
        }

        if (Blend != 0)
        {
            /* Draw each pixel in the current horizontal line in turn,
                starting with the leftmost one */
            for (X = HLinePtr->XStart; X <= HLinePtr->XEnd; X++)
                VBufWriteBlendColor(vbuf, Y * screen.pitch + X, Color, Blend);
        }
        else
        {
            /* Draw each pixel in the current horizontal line in turn,
                starting with the leftmost one */
            for (X = HLinePtr->XStart; X <= HLinePtr->XEnd; X++)
                VBufWriteColor(vbuf, Y * screen.pitch + X, Color);
        }
    }
}

static void ScanEdge(int X1, int Y1, int X2, int Y2, int SetXStart,
    int SkipFirst, struct HLine **EdgePointPtr)
{
    int Y, DeltaX, Height, Width, AdvanceAmt, ErrorTerm, i;
    int ErrorTermAdvance, XMajorAdvanceAmt;
    struct HLine *WorkingEdgePointPtr;

    WorkingEdgePointPtr = *EdgePointPtr; /* avoid double dereference */
    AdvanceAmt = ((DeltaX = X2 - X1) > 0) ? 1 : -1;
                                /* direction in which X moves (Y2 is
                                always > Y1, so Y always counts up) */

    if ((Height = Y2 - Y1) <= 0)  /* Y length of the edge */
        return;     /* guard against 0-length and horizontal edges */

    /* Figure out whether the edge is vertical, diagonal, X-major
        (mostly horizontal), or Y-major (mostly vertical) and handle
        appropriately */
    if ((Width = abs(DeltaX)) == 0) {
        /* The edge is vertical; special-case by just storing the same
            X coordinate for every scan line */
        /* Scan the edge for each scan line in turn */
        for (i = Height - SkipFirst; i-- > 0; WorkingEdgePointPtr++) {
            /* Store the X coordinate in the appropriate edge list */
            if (SetXStart == 1)
                WorkingEdgePointPtr->XStart = X1;
            else
                WorkingEdgePointPtr->XEnd = X1;
        }
    } else if (Width == Height) {
        /* The edge is diagonal; special-case by advancing the X
            coordinate 1 pixel for each scan line */
        if (SkipFirst) /* skip the first point if so indicated */
            X1 += AdvanceAmt; /* move 1 pixel to the left or right */
        /* Scan the edge for each scan line in turn */
        for (i = Height - SkipFirst; i-- > 0; WorkingEdgePointPtr++) {
            /* Store the X coordinate in the appropriate edge list */
            if (SetXStart == 1)
                WorkingEdgePointPtr->XStart = X1;
            else
                WorkingEdgePointPtr->XEnd = X1;
            X1 += AdvanceAmt; /* move 1 pixel to the left or right */
        }
    } else if (Height > Width) {
        /* Edge is closer to vertical than horizontal (Y-major) */
        if (DeltaX >= 0)
            ErrorTerm = 0; /* initial error term going left->right */
        else
            ErrorTerm = -Height + 1;   /* going right->left */
        if (SkipFirst) {   /* skip the first point if so indicated */
            /* Determine whether it's time for the X coord to advance */
            if ((ErrorTerm += Width) > 0) {
                X1 += AdvanceAmt; /* move 1 pixel to the left or right */
                ErrorTerm -= Height; /* advance ErrorTerm to next point */
            }
        }
        /* Scan the edge for each scan line in turn */
        for (i = Height - SkipFirst; i-- > 0; WorkingEdgePointPtr++) {
            /* Store the X coordinate in the appropriate edge list */
            if (SetXStart == 1)
                WorkingEdgePointPtr->XStart = X1;
            else
                WorkingEdgePointPtr->XEnd = X1;
            /* Determine whether it's time for the X coord to advance */
            if ((ErrorTerm += Width) > 0) {
                X1 += AdvanceAmt; /* move 1 pixel to the left or right */
                ErrorTerm -= Height; /* advance ErrorTerm to correspond */
            }
        }
    } else {
        /* Edge is closer to horizontal than vertical (X-major) */
        /* Minimum distance to advance X each time */
        XMajorAdvanceAmt = (Width / Height) * AdvanceAmt;
        /* Error term advance for deciding when to advance X 1 extra */
        ErrorTermAdvance = Width % Height;
        if (DeltaX >= 0)
            ErrorTerm = 0; /* initial error term going left->right */
        else
            ErrorTerm = -Height + 1;   /* going right->left */
        if (SkipFirst) {   /* skip the first point if so indicated */
            X1 += XMajorAdvanceAmt;    /* move X minimum distance */
            /* Determine whether it's time for X to advance one extra */
            if ((ErrorTerm += ErrorTermAdvance) > 0) {
                X1 += AdvanceAmt;       /* move X one more */
                ErrorTerm -= Height; /* advance ErrorTerm to correspond */
            }
        }
        /* Scan the edge for each scan line in turn */
        for (i = Height - SkipFirst; i-- > 0; WorkingEdgePointPtr++) {
            /* Store the X coordinate in the appropriate edge list */
            if (SetXStart == 1)
                WorkingEdgePointPtr->XStart = X1;
            else
                WorkingEdgePointPtr->XEnd = X1;
            X1 += XMajorAdvanceAmt;    /* move X minimum distance */
            /* Determine whether it's time for X to advance one extra */
            if ((ErrorTerm += ErrorTermAdvance) > 0) {
                X1 += AdvanceAmt;       /* move X one more */
                ErrorTerm -= Height; /* advance ErrorTerm to correspond */
            }
        }
    }

    *EdgePointPtr = WorkingEdgePointPtr;   /* advance caller's ptr */
}

int FillConvexPolygon(struct PointListHeader * VertexList, int Color,
    int XOffset, int YOffset, unsigned char Blend)
{
    int i, MinIndexL, MaxIndex, MinIndexR, SkipFirst, Temp;
    int MinPoint_Y, MaxPoint_Y, TopIsFlat, LeftEdgeDir;
    int NextIndex, CurrentIndex, PreviousIndex;
    int DeltaXN, DeltaYN, DeltaXP, DeltaYP;
    struct HLineList WorkingHLineList;
    struct HLine *EdgePointPtr;
    struct PolyPoint *VertexPtr;

    /* Point to the vertex list */
    VertexPtr = VertexList->PointPtr;

    /* Scan the list to find the top and bottom of the polygon */
    if (VertexList->Length == 0)
        return(1);  /* reject null polygons */
    MaxPoint_Y = MinPoint_Y = VertexPtr[MinIndexL = MaxIndex = 0].Y;
    for (i = 1; i < VertexList->Length; i++) {
        if (VertexPtr[i].Y < MinPoint_Y)
            MinPoint_Y = VertexPtr[MinIndexL = i].Y; /* new top */
        else if (VertexPtr[i].Y > MaxPoint_Y)
            MaxPoint_Y = VertexPtr[MaxIndex = i].Y; /* new bottom */
    }
    if (MinPoint_Y == MaxPoint_Y)
        return(1);  /* polygon is 0-height; avoid infinite loop below */

    /* Scan in ascending order to find the last top-edge point */
    MinIndexR = MinIndexL;
    while (VertexPtr[MinIndexR].Y == MinPoint_Y)
        INDEX_FORWARD(MinIndexR);
    INDEX_BACKWARD(MinIndexR); /* back up to last top-edge point */

    /* Now scan in descending order to find the first top-edge point */
    while (VertexPtr[MinIndexL].Y == MinPoint_Y)
        INDEX_BACKWARD(MinIndexL);
    INDEX_FORWARD(MinIndexL); /* back up to first top-edge point */

    /* Figure out which direction through the vertex list from the top
        vertex is the left edge and which is the right */
    LeftEdgeDir = -1; /* assume left edge runs down thru vertex list */
    if ((TopIsFlat = (VertexPtr[MinIndexL].X !=
            VertexPtr[MinIndexR].X) ? 1 : 0) == 1) {
        /* If the top is flat, just see which of the ends is leftmost */
        if (VertexPtr[MinIndexL].X > VertexPtr[MinIndexR].X) {
            LeftEdgeDir = 1;  /* left edge runs up through vertex list */
            Temp = MinIndexL;       /* swap the indices so MinIndexL   */
            MinIndexL = MinIndexR;  /* points to the start of the left */
            MinIndexR = Temp;       /* edge, similarly for MinIndexR   */
        }
    } else {
        /* Point to the downward end of the first line of each of the
            two edges down from the top */
        NextIndex = MinIndexR;
        INDEX_FORWARD(NextIndex);
        PreviousIndex = MinIndexL;
        INDEX_BACKWARD(PreviousIndex);
        /* Calculate X and Y lengths from the top vertex to the end of
            the first line down each edge; use those to compare slopes
            and see which line is leftmost */
        DeltaXN = VertexPtr[NextIndex].X - VertexPtr[MinIndexL].X;
        DeltaYN = VertexPtr[NextIndex].Y - VertexPtr[MinIndexL].Y;
        DeltaXP = VertexPtr[PreviousIndex].X - VertexPtr[MinIndexL].X;
        DeltaYP = VertexPtr[PreviousIndex].Y - VertexPtr[MinIndexL].Y;
        if (((long)DeltaXN * DeltaYP - (long)DeltaYN * DeltaXP) < 0L) {
            LeftEdgeDir = 1;  /* left edge runs up through vertex list */
            Temp = MinIndexL;       /* swap the indices so MinIndexL   */
            MinIndexL = MinIndexR;  /* points to the start of the left */
            MinIndexR = Temp;       /* edge, similarly for MinIndexR   */
        }
    }

    /* Set the # of scan lines in the polygon, skipping the bottom edge
        and also skipping the top vertex if the top isn't flat because
        in that case the top vertex has a right edge component, and set
        the top scan line to draw, which is likewise the second line of
        the polygon unless the top is flat */
    if ((WorkingHLineList.Length =
            MaxPoint_Y - MinPoint_Y - 1 + TopIsFlat) <= 0)
        return(1);  /* there's nothing to draw, so we're done */
    WorkingHLineList.YStart = YOffset + MinPoint_Y + 1 - TopIsFlat;

    /* Get memory in which to store the line list we generate */
    if ((WorkingHLineList.HLinePtr =
            (struct HLine *) (malloc(sizeof(struct HLine) *
            WorkingHLineList.Length))) == NULL)
        return(0);  /* couldn't get memory for the line list */

    /* Scan the left edge and store the boundary points in the list */
    /* Initial pointer for storing scan converted left-edge coords */
    EdgePointPtr = WorkingHLineList.HLinePtr;
    /* Start from the top of the left edge */
    PreviousIndex = CurrentIndex = MinIndexL;
    /* Skip the first point of the first line unless the top is flat;
        if the top isn't flat, the top vertex is exactly on a right
        edge and isn't drawn */
    SkipFirst = TopIsFlat ? 0 : 1;
    /* Scan convert each line in the left edge from top to bottom */
    do {
        INDEX_MOVE(CurrentIndex,LeftEdgeDir);
        ScanEdge(VertexPtr[PreviousIndex].X + XOffset,
                VertexPtr[PreviousIndex].Y,
                VertexPtr[CurrentIndex].X + XOffset,
                VertexPtr[CurrentIndex].Y, 1, SkipFirst, &EdgePointPtr);
        PreviousIndex = CurrentIndex;
        SkipFirst = 0; /* scan convert the first point from now on */
    } while (CurrentIndex != MaxIndex);

    /* Scan the right edge and store the boundary points in the list */
    EdgePointPtr = WorkingHLineList.HLinePtr;
    PreviousIndex = CurrentIndex = MinIndexR;
    SkipFirst = TopIsFlat ? 0 : 1;
    /* Scan convert the right edge, top to bottom. X coordinates are
        adjusted 1 to the left, effectively causing scan conversion of
        the nearest points to the left of but not exactly on the edge */
    do {
        INDEX_MOVE(CurrentIndex,-LeftEdgeDir);
        ScanEdge(VertexPtr[PreviousIndex].X + XOffset - 1,
                VertexPtr[PreviousIndex].Y,
                VertexPtr[CurrentIndex].X + XOffset - 1,
                VertexPtr[CurrentIndex].Y, 0, SkipFirst, &EdgePointPtr);
        PreviousIndex = CurrentIndex;
        SkipFirst = 0; /* scan convert the first point from now on */
    } while (CurrentIndex != MaxIndex);

    /* Draw the line list representing the scan converted polygon */
    DrawHorizontalLineList(&WorkingHLineList, Color, Blend);

    /* Release the line list's memory and we're successfully done */
    free(WorkingHLineList.HLinePtr);
    return(1);
}

int FillConvexPolygonEx(int Length, struct PolyPoint *PointPtr,
    int Color, unsigned char Blend)
{
    struct PointListHeader VertexList;
    VertexList.Length = Length;
    VertexList.PointPtr = PointPtr;
    return FillConvexPolygon(&VertexList, Color, 0, 0, Blend);
}
#endif