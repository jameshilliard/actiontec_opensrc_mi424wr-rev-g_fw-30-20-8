/* DO NOT EDIT THIS FILE - it is machine generated */
#include <native.h>

#ifndef _Included_java_util_Vector
#define _Included_java_util_Vector

#ifdef __cplusplus
extern "C" {
#endif

/* Header for class java_util_Vector */

typedef struct Hjava_util_Vector {
  /* Fields from java/lang/Object: */
  Hjava_lang_Object base;

  /* Fields from java/util/AbstractCollection: */

  /* Fields from java/util/AbstractList: */
  jint modCount;

  /* Fields from java/util/Vector: */
#undef java_util_Vector_serialVersionUID
#define java_util_Vector_serialVersionUID -2767605614048989439LL
  HArrayOfObject* elementData;
  jint elementCount;
  jint capacityIncrement;
} Hjava_util_Vector;


#ifdef __cplusplus
}
#endif

#endif
