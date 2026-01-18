/*
 * WARNING: If a .h file depends on other
 * .h files, it must be listed ABOVE
 * the files it depends on in this file
 * i.e. pdf.h, which depends on strings.h,
 * must be included before strings.h
 */
#define PDF_IMPLEMENTATION
#include "../pdf.h"
#define DICTIONARY_IMPLEMENTATION
#include "../dictionary.h"
#define STRING_LIST_IMPLEMENTATION
#include "../string_list.h"

#define MY_STRINGS_IMPLEMENTATION
#include "../strings.h"

#define ARENAS_IMPLEMENTATION
#include "../arenas.h"
#define CASTS_IMPLEMENTATION
#include "../casts.h"
#define PRETTY_PRINT_IMPLEMENTATION
#include "../pretty_print.h"
#define GENERIC_TYPE_DYNAMIC_ARRAY_IMPLEMENTATION
#include "../templates/dynamic_array.h"
