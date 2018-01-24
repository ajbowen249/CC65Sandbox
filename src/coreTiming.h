#ifndef __CORETIMINGS_H
#define __CORETIMINGS_H

typedef void (*iterruptCallback)(void);

void setupIsr(iterruptCallback rasterCallback);

#endif // __CORETIMINGS_H
