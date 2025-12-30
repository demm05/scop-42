#pragma once

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* * This function is automatically picked up by LeakSanitizer.
 * Use it to ignore leaks in system libraries that you cannot control.
 */
const char *__lsan_default_suppressions() {
  return "leak:libnvidia-glcore.so\n"
         "leak:libnvidia\n"
         "leak:nvidia\n"
         "leak:<unknown module>\n"
         "leak:libfontconfig.so\n"
         "leak:libwayland-client.so\n"
         "leak:libdbus-1.so\n"
         "leak:libgtk-3.so\n"
         "leak:libglib-2.0.so\n"
         "leak:libdecor-gtk.so\n"
         "leak:libgobject-2.0.so\n";
}

extern "C" const char *__asan_default_options() {
  return "print_suppressions=0";
}

#ifdef __cplusplus
}
#endif
