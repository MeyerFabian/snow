#ifndef LEFT_SUPPORT
#ifdef P2G_PULL
// left and right swap due to pulling
#define LEFT_SUPPORT 2
#define RIGHT_SUPPORT 1
#else
#define LEFT_SUPPORT 1
#define RIGHT_SUPPORT 2
#endif
#endif

