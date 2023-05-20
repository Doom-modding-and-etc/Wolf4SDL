

#ifdef RESET_IOP
extern void resetIOP();
#else
extern void initRPC();
#endif
extern void PS2_Load_Modules(void);
extern void PS2_Unload_Modules(void);
extern void PS2_Load_RPC_Patches();
extern void PS2_Unload_RPC_Patches(void);