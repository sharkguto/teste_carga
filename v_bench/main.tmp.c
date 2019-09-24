 

#include <stdio.h>  // TODO remove all these includes, define all function signatures and types manually
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h> // for va_list
#include <inttypes.h>  // int64_t etc
#include <string.h> // memcpy

#ifndef _WIN32
#include <ctype.h>
#include <locale.h> // tolower
#include <sys/time.h>
#include <unistd.h> // sleep	
#endif


#ifdef __APPLE__
#include <libproc.h> // proc_pidpath
#include <execinfo.h> // backtrace and backtrace_symbols_fd
#endif

#ifdef __linux__
#ifndef __BIONIC__
#include <execinfo.h> // backtrace and backtrace_symbols_fd
#endif
#pragma weak backtrace
#pragma weak backtrace_symbols_fd
#endif


#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h> // os__wait uses wait on nix
#endif

#define EMPTY_STRUCT_DECLARATION
#define EMPTY_STRUCT_INITIALIZATION 0
#ifdef __TINYC__
#undef EMPTY_STRUCT_INITIALIZATION
#define EMPTY_STRUCT_INITIALIZATION
#endif

#define OPTION_CAST(x) (x)

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE
#include <windows.h>

// must be included after <windows.h>
#ifndef __TINYC__
#include <shellapi.h>
#endif

#include <io.h> // _waccess
#include <fcntl.h> // _O_U8TEXT
#include <direct.h> // _wgetcwd
//#include <WinSock2.h>
#ifdef _MSC_VER
// On MSVC these are the same (as long as /volatile:ms is passed)
#define _Atomic volatile

// MSVC cannot parse some things properly
#undef EMPTY_STRUCT_DECLARATION
#undef OPTION_CAST

#define EMPTY_STRUCT_DECLARATION int ____dummy_variable
#define OPTION_CAST(x)
#endif

void pthread_mutex_lock(HANDLE *m) {
	WaitForSingleObject(*m, INFINITE);
}

void pthread_mutex_unlock(HANDLE *m) {
	ReleaseMutex(*m);
}
#else
#include <pthread.h>
#endif

//================================== TYPEDEFS ================================*/

typedef int64_t i64;
typedef int16_t i16;
typedef int8_t i8;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t byte;
typedef uint32_t rune;
typedef float f32;
typedef double f64;
typedef unsigned char* byteptr;
typedef int* intptr;
typedef void* voidptr;
typedef struct array array;
typedef struct map map;
typedef array array_string;
typedef array array_int;
typedef array array_byte;
typedef array array_f32;
typedef array array_f64;
typedef map map_int;
typedef map map_string;
#ifndef bool
	typedef int bool;
	#define true 1
	#define false 0
#endif

//============================== HELPER C MACROS =============================*/
#define _PUSH(arr, val, tmp, tmp_typ) {tmp_typ tmp = (val); array__push(arr, &tmp);}
#define _PUSH_MANY(arr, val, tmp, tmp_typ) {tmp_typ tmp = (val); array__push_many(arr, tmp.data, tmp.len);}
#define _IN(typ, val, arr) array_##typ##_contains(arr, val)
#define _IN_MAP(val, m) map__exists(m, val)
//================================== GLOBALS =================================*/
byteptr g_str_buf;
int load_so(byteptr);
void reload_so();
void init_consts();


 int g_test_ok = 1; 
 this line will be replaced with definitions
 array new_array(int mylen, int cap, int elm_size) {
 
array arr= (array) { .len =  mylen , .cap =  cap , .element_size =  elm_size , .data =  v_calloc ( cap * elm_size ) } ;
 
return  arr ;
 
 }
 array _make(int len, int cap, int elm_size) {
 
return  new_array ( len , cap , elm_size ) ;
 
 }
 array new_array_from_c_array(int len, int cap, int elm_size, void* c_array) {
 
array arr= (array) { .len =  len , .cap =  cap , .element_size =  elm_size , .data =  v_malloc ( cap * elm_size ) } ;
 
 memcpy ( arr .data ,  c_array ,  len * elm_size ) ;
 
return  arr ;
 
 }
 array new_array_from_c_array_no_alloc(int len, int cap, int elm_size, void* c_array) {
 
array arr= (array) { .len =  len , .cap =  cap , .element_size =  elm_size , .data =  c_array } ;
 
return  arr ;
 
 }
 array array_repeat_old(void* val, int nr_repeats, int elm_size) {
 
array arr= (array) { .len =  nr_repeats , .cap =  nr_repeats , .element_size =  elm_size , .data =  v_malloc ( nr_repeats * elm_size ) } ;
 
 for (
int i= 0  ;  i < nr_repeats  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * elm_size ,  val ,  elm_size ) ;
 
 }
 ;
 
return  arr ;
 
 }
 array array_repeat(array a, int nr_repeats) {
 
array arr= (array) { .len =  nr_repeats , .cap =  nr_repeats , .element_size =  a .element_size , .data =  v_malloc ( nr_repeats * a .element_size ) } ;
 
void* val=(byte*) a .data + 0 ;
 
 for (
int i= 0  ;  i < nr_repeats  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * a .element_size ,  val ,  a .element_size ) ;
 
 }
 ;
 
return  arr ;
 
 }
 void array_sort_with_compare(array* a, void* compare) {
 
 qsort ( a ->data ,  a ->len ,  a ->element_size ,  compare ) ;
 
 }
 void array_insert(array* a, int i, void* val) {
 
 if ( i >= a ->len ) {
 
 v_panic ( tos2((byte*)"array.insert: index larger than length") ) ;
 
 }
 ;
 
 array__push( a , val ) ;
 
int size= a ->element_size ;
 
 memmove ((byte*) a ->data + ( i + 1 ) * size , (byte*) a ->data + i * size ,  ( a ->len - i ) * size ) ;
 
 array_set( a , i , val ) ;
 
 }
 void array_prepend(array* a, void* val) {
 
 array_insert( a , 0 , val ) ;
 
 }
 void v_array_delete(array* a, int idx) {
 
int size= a ->element_size ;
 
 memmove ((byte*) a ->data + idx * size , (byte*) a ->data + ( idx + 1 ) * size ,  ( a ->len - idx ) * size ) ;
 
 a ->len -- ;
 
 a ->cap -- ;
 
 }
 void* array__get(array a, int i) {
 
 if ( i < 0  ||  i >= a .len ) {
 
 v_panic ( _STR("array index out of range: %d/%d", i, a .len) ) ;
 
 }
 ;
 
return (byte*) a .data + i * a .element_size ;
 
 }
 void* array_first(array a) {
 
 if ( a .len == 0 ) {
 
 v_panic ( tos2((byte*)"array.first: empty array") ) ;
 
 }
 ;
 
return (byte*) a .data + 0 ;
 
 }
 void* array_last(array a) {
 
 if ( a .len == 0 ) {
 
 v_panic ( tos2((byte*)"array.last: empty array") ) ;
 
 }
 ;
 
return (byte*) a .data + ( a .len - 1 ) * a .element_size ;
 
 }
 array array_left(array s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  array_slice( s , 0 , n ) ;
 
 }
 array array_right(array s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  array_slice( s , n , s .len ) ;
 
 }
 array array_slice(array s, int start, int _end) {
 
int end= _end ;
 
 if ( start > end ) {
 
 v_panic ( _STR("invalid slice index: %d > %d", start, end) ) ;
 
 }
 ;
 
 if ( end > s .len ) {
 
 v_panic ( _STR("runtime error: slice bounds out of range (%d >= %d)", end, s .len) ) ;
 
 }
 ;
 
 if ( start < 0 ) {
 
 v_panic ( _STR("runtime error: slice bounds out of range (%d < 0)", start) ) ;
 
 }
 ;
 
int l= end - start ;
 
array res= (array) { .element_size =  s .element_size , .data = (byte*) s .data + start * s .element_size , .len =  l , .cap =  l } ;
 
return  res ;
 
 }
 void array_set(array* a, int idx, void* val) {
 
 if ( idx < 0  ||  idx >= a ->len ) {
 
 v_panic ( _STR("array index out of range: %d / %d", idx, a ->len) ) ;
 
 }
 ;
 
 memcpy ((byte*) a ->data + a ->element_size * idx ,  val ,  a ->element_size ) ;
 
 }
 void array__push(array* arr, void* val) {
 
 if ( arr ->len >= arr ->cap - 1 ) {
 
int cap= ( arr ->len + 1 ) * 2 ;
 
 if ( arr ->cap == 0 ) {
 
 arr ->data  =  v_malloc ( cap * arr ->element_size ) ;
 
 }
  else { 
 
 arr ->data  =  realloc ( arr ->data ,  cap * arr ->element_size ) ;
 
 }
 ;
 
 arr ->cap  =  cap ;
 
 }
 ;
 
 memcpy ((byte*) arr ->data + arr ->element_size * arr ->len ,  val ,  arr ->element_size ) ;
 
 arr ->len ++ ;
 
 }
 void array__push_many(array* arr, void* val, int size) {
 
 if ( arr ->len >= arr ->cap - size ) {
 
int cap= ( arr ->len + size ) * 2 ;
 
 if ( arr ->cap == 0 ) {
 
 arr ->data  =  v_malloc ( cap * arr ->element_size ) ;
 
 }
  else { 
 
 arr ->data  =  realloc ( arr ->data ,  cap * arr ->element_size ) ;
 
 }
 ;
 
 arr ->cap  =  cap ;
 
 }
 ;
 
 memcpy ((byte*) arr ->data + arr ->element_size * arr ->len ,  val ,  arr ->element_size * size ) ;
 
 arr ->len  +=  size ;
 
 }
 array array_reverse(array a) {
 
array arr= (array) { .len =  a .len , .cap =  a .cap , .element_size =  a .element_size , .data =  v_malloc ( a .cap * a .element_size ) } ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * arr .element_size ,  & /*v*/  ( *(array*) array__get( a , a .len - 1 - i) ) ,  arr .element_size ) ;
 
 }
 ;
 
return  arr ;
 
 }
 array array_clone(array a) {
 
array arr= (array) { .len =  a .len , .cap =  a .cap , .element_size =  a .element_size , .data =  v_malloc ( a .cap * a .element_size ) } ;
 
 memcpy ( arr .data ,  a .data ,  a .cap * a .element_size ) ;
 
return  arr ;
 
 }
 void v_array_free(array a) {
 
 free ( a .data ) ;
 
 }
 string array_string_str(array_string a) {
 
strings__Builder sb= strings__new_builder ( a .len * 3 ) ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"[") ) ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
string val= ( *(string*) array__get( a , i) ) ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"\"") ) ;
 
 strings__Builder_write(& /* ? */ sb , val ) ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"\"") ) ;
 
 if ( i < a .len - 1 ) {
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)", ") ) ;
 
 }
 ;
 
 }
 ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"]") ) ;
 
return  strings__Builder_str( sb ) ;
 
 }
 string array_byte_hex(array_byte b) {
 
byte* hex= v_malloc ( b .len * 2 + 1 ) ;
 
byte* ptr= & /*v*/  hex [/*ptr*/ 0 ]/*rbyte 1*/ ;
 
 for (
int i= 0  ;  i < b .len  ;  i ++ ) { 
 
 
 ptr  +=  sprintf ( ((char*)( ptr ) ) ,  "%02x" ,  ( *(byte*) array__get( b , i) ) ) ;
 
 }
 ;
 
return  (tos2((byte *) hex ) ) ;
 
 }
 int copy(array_byte dst, array_byte src) {
 
 if ( dst .len > 0  &&  src .len > 0 ) {
 
int min= ( dst .len < src .len ) ? ( dst .len ) : ( src .len ) ;
 
 memcpy ( dst .data ,  array_left( src , min ) .data ,  dst .element_size * min ) ;
 
return  min ;
 
 }
 ;
 
return  0 ;
 
 }
 int compare_ints(int* a, int* b) {
 
 if ( a < b ) {
 
return  - 1 ;
 
 }
 ;
 
 if ( a > b ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 }
 void array_int_sort(array_int* a) {
 
 array_sort_with_compare( a , compare_ints ) ;
 
 }
 int vstrlen(byte* s) {
 
return  strlen ( ((char*)( s ) ) ) ;
 
 }
 void todo() {
 
 }
 string tos(byte* s, int len) {
 
 if ( isnil ( s ) ) {
 
 v_panic ( tos2((byte*)"tos(): nil string") ) ;
 
 }
 ;
 
return  (string) { .str =  s , .len =  len } ;
 
 }
 string tos_clone(byte* s) {
 
 if ( isnil ( s ) ) {
 
 v_panic ( tos2((byte*)"tos: nil string") ) ;
 
 }
 ;
 
return  string_clone( tos2 ( s ) ) ;
 
 }
 string tos2(byte* s) {
 
 if ( isnil ( s ) ) {
 
 v_panic ( tos2((byte*)"tos2: nil string") ) ;
 
 }
 ;
 
int len= vstrlen ( s ) ;
 
string res= tos ( s , len ) ;
 
return  res ;
 
 }
 string string_clone(string a) {
 
string b= (string) { .len =  a .len , .str =  v_malloc ( a .len + 1 ) } ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
 b .str[ i ]/*rbyte 1*/  =  a .str[ i ]/*rbyte 0*/ ;
 
 }
 ;
 
 b .str[ a .len ]/*rbyte 1*/  =  '\0' ;
 
return  b ;
 
 }
 string string_replace(string s, string rep, string with) {
 
 if ( s .len == 0  ||  rep .len == 0 ) {
 
return  s ;
 
 }
 ;
 
array_int idxs=new_array_from_c_array(0, 0, sizeof(int), (int[]) {   0 }) ;
 
string rem= s ;
 
int rstart= 0 ;
 
 while (1) { 
 
int i= string_index( rem , rep ) ;
 
 if ( i < 0 ) {
 
 break
 ;
 
 }
 ;
 
_PUSH(& idxs , ( rstart + i ), tmp9, int) ;
 
 i  +=  rep .len ;
 
 rstart  +=  i ;
 
 rem  =  string_substr( rem , i , rem .len ) ;
 
 }
 ;
 
 if ( idxs .len == 0 ) {
 
return  s ;
 
 }
 ;
 
int new_len= s .len + idxs .len * ( with .len - rep .len ) ;
 
byte* b= v_malloc ( new_len + 1 ) ;
 
int idx_pos= 0 ;
 
int cur_idx= ( *(int*) array__get( idxs , idx_pos) ) ;
 
int b_i= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( i == cur_idx ) {
 
 for (
int j= 0  ;  j < with .len  ;  j ++ ) { 
 
 
 b [/*ptr*/ b_i ]/*rbyte 1*/  =  with .str[ j ]/*rbyte 0*/ ;
 
 b_i ++ ;
 
 }
 ;
 
 i  +=  rep .len - 1 ;
 
 idx_pos ++ ;
 
 if ( idx_pos < idxs .len ) {
 
 cur_idx  =  ( *(int*) array__get( idxs , idx_pos) ) ;
 
 }
 ;
 
 }
  else { 
 
 b [/*ptr*/ b_i ]/*rbyte 1*/  =  s .str[ i ]/*rbyte 0*/ ;
 
 b_i ++ ;
 
 }
 ;
 
 }
 ;
 
 b [/*ptr*/ new_len ]/*rbyte 1*/  =  '\0' ;
 
return  tos ( b , new_len ) ;
 
 }
 int v_string_int(string s) {
 
return  atoi ( ((char*)( s .str ) ) ) ;
 
 }
 i64 string_i64(string s) {
 
return  atoll ( ((char*)( s .str ) ) ) ;
 
 }
 f32 string_f32(string s) {
 
return  atof ( ((char*)( s .str ) ) ) ;
 
 }
 f64 string_f64(string s) {
 
return  atof ( ((char*)( s .str ) ) ) ;
 
 }
 u32 string_u32(string s) {
 
return  strtoul ( ((char*)( s .str ) ) ,  0 ,  0 ) ;
 
 }
 u64 string_u64(string s) {
 
return  strtoull ( ((char*)( s .str ) ) ,  0 ,  0 ) ;
 
 }
 bool string_eq(string s, string a) {
 
 if ( isnil ( s .str ) ) {
 
 v_panic ( tos2((byte*)"string.eq(): nil string") ) ;
 
 }
 ;
 
 if ( s .len != a .len ) {
 
return  0 ;
 
 }
 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( s .str[ i ]/*rbyte 0*/ != a .str[ i ]/*rbyte 0*/ ) {
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
return  1 ;
 
 }
 bool string_ne(string s, string a) {
 
return  ! string_eq( s , a ) ;
 
 }
 bool string_lt(string s, string a) {
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( i >= a .len  ||  s .str[ i ]/*rbyte 0*/ > a .str[ i ]/*rbyte 0*/ ) {
 
return  0 ;
 
 }
  else  if ( s .str[ i ]/*rbyte 0*/ < a .str[ i ]/*rbyte 0*/ ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
 if ( s .len < a .len ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 }
 bool string_le(string s, string a) {
 
return  string_lt( s , a )  ||  string_eq( s , a ) ;
 
 }
 bool string_gt(string s, string a) {
 
return  ! string_le( s , a ) ;
 
 }
 bool string_ge(string s, string a) {
 
return  ! string_lt( s , a ) ;
 
 }
 string string_add(string s, string a) {
 
int new_len= a .len + s .len ;
 
string res= (string) { .len =  new_len , .str =  v_malloc ( new_len + 1 ) } ;
 
 for (
int j= 0  ;  j < s .len  ;  j ++ ) { 
 
 
 res .str[ j ]/*rbyte 1*/  =  s .str[ j ]/*rbyte 0*/ ;
 
 }
 ;
 
 for (
int j= 0  ;  j < a .len  ;  j ++ ) { 
 
 
 res .str[ s .len + j ]/*rbyte 1*/  =  a .str[ j ]/*rbyte 0*/ ;
 
 }
 ;
 
 res .str[ new_len ]/*rbyte 1*/  =  '\0' ;
 
return  res ;
 
 }
 array_string string_split(string s, string delim) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 if ( delim .len == 0 ) {
 
_PUSH(& res , ( s ), tmp28, string) ;
 
return  res ;
 
 }
 ;
 
 if ( delim .len == 1 ) {
 
return  string_split_single( s , delim .str[ 0 ]/*rbyte 0*/ ) ;
 
 }
 ;
 
int i= 0 ;
 
int start= 0 ;
 
 while ( i < s .len ) {
 
 
bool a= s .str[ i ]/*rbyte 0*/ == delim .str[ 0 ]/*rbyte 0*/ ;
 
int j= 1 ;
 
 while ( j < delim .len  &&  a ) {
 
 
 a  =  a  &&  s .str[ i + j ]/*rbyte 0*/ == delim .str[ j ]/*rbyte 0*/ ;
 
 j ++ ;
 
 }
 ;
 
bool last= i == s .len - 1 ;
 
 if ( a  ||  last ) {
 
 if ( last ) {
 
 i ++ ;
 
 }
 ;
 
string val= string_substr( s , start , i ) ;
 
 if ( val .len > 0 ) {
 
 if ( string_starts_with( val , delim ) ) {
 
 val  =  string_right( val , delim .len ) ;
 
 }
 ;
 
_PUSH(& res , ( string_trim_space( val ) ), tmp35, string) ;
 
 }
 ;
 
 start  =  i ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  res ;
 
 }
 array_string string_split_single(string s, byte delim) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 if ( ((int)( delim ) ) == 0 ) {
 
_PUSH(& res , ( s ), tmp37, string) ;
 
return  res ;
 
 }
 ;
 
int i= 0 ;
 
int start= 0 ;
 
 while ( i < s .len ) {
 
 
bool is_delim= s .str[ i ]/*rbyte 0*/ == delim ;
 
bool last= i == s .len - 1 ;
 
 if ( is_delim  ||  last ) {
 
 if ( ! is_delim  &&  i == s .len - 1 ) {
 
 i ++ ;
 
 }
 ;
 
string val= string_substr( s , start , i ) ;
 
 if ( val .len > 0 ) {
 
_PUSH(& res , ( val ), tmp43, string) ;
 
 }
 ;
 
 start  =  i + 1 ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  res ;
 
 }
 array_string string_split_into_lines(string s) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 if ( s .len == 0 ) {
 
return  res ;
 
 }
 ;
 
int start= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
bool last= i == s .len - 1 ;
 
 if ( ((int)( s .str[ i ]/*rbyte 0*/ ) ) == 10  ||  last ) {
 
 if ( last ) {
 
 i ++ ;
 
 }
 ;
 
string line= string_substr( s , start , i ) ;
 
_PUSH(& res , ( line ), tmp49, string) ;
 
 start  =  i + 1 ;
 
 }
 ;
 
 }
 ;
 
return  res ;
 
 }
 string string_left(string s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  string_substr( s , 0 , n ) ;
 
 }
 string string_right(string s, int n) {
 
 if ( n >= s .len ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_substr( s , n , s .len ) ;
 
 }
 string string_substr(string s, int start, int end) {
 
 if ( start > end  ||  start > s .len  ||  end > s .len  ||  start < 0  ||  end < 0 ) {
 
 v_panic ( _STR("substr(%d, %d) out of bounds (len=%d)", start, end, s .len) ) ;
 
 }
 ;
 
int len= end - start ;
 
string res= (string) { .len =  len , .str =  v_malloc ( len + 1 ) } ;
 
 for (
int i= 0  ;  i < len  ;  i ++ ) { 
 
 
 res .str [/*ptr*/ i ]/*rbyte 1*/  =  s .str [/*ptr*/ start + i ]/*rbyte 0*/ ;
 
 }
 ;
 
 res .str [/*ptr*/ len ]/*rbyte 1*/  =  '\0' ;
 
return  res ;
 
 }
 int string_index_old(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int i= 0 ;
 
 while ( i < s .len ) {
 
 
int j= 0 ;
 
int ii= i ;
 
 while ( j < p .len  &&  s .str[ ii ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 
 j ++ ;
 
 ii ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
return  i - p .len + 1 ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  - 1 ;
 
 }
 int string_index(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
array_int prefix= array_repeat(new_array_from_c_array(1, 1, sizeof(int), (int[]) {  0  }) , p .len ) ;
 
int j= 0 ;
 
 for (
int i= 1  ;  i < p .len  ;  i ++ ) { 
 
 
 while ( p .str[ j ]/*rbyte 0*/ != p .str[ i ]/*rbyte 0*/  &&  j > 0 ) {
 
 
 j  =  ( *(int*) array__get( prefix , j - 1) ) ;
 
 }
 ;
 
 if ( p .str[ j ]/*rbyte 0*/ == p .str[ i ]/*rbyte 0*/ ) {
 
 j ++ ;
 
 }
 ;
 
array_set(&/*q*/ prefix , i , & (int []) {  j }) ;
 
 }
 ;
 
 j  =  0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 while ( p .str[ j ]/*rbyte 0*/ != s .str[ i ]/*rbyte 0*/  &&  j > 0 ) {
 
 
 j  =  ( *(int*) array__get( prefix , j - 1) ) ;
 
 }
 ;
 
 if ( p .str[ j ]/*rbyte 0*/ == s .str[ i ]/*rbyte 0*/ ) {
 
 j ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
return  i - p .len + 1 ;
 
 }
 ;
 
 }
 ;
 
return  - 1 ;
 
 }
 int string_index_any(string s, string chars) {
 
 string tmp64 =  chars;
 array_byte bytes_tmp64 = string_bytes( tmp64 );
 ;
for (int tmp65 = 0; tmp65 < tmp64 .len; tmp65 ++) {
 byte c = ((byte *) bytes_tmp64 . data)[tmp65];
 
 
int index= string_index( s , byte_str( c ) ) ;
 
 if ( index != - 1 ) {
 
return  index ;
 
 }
 ;
 
 }
 ;
 
return  - 1 ;
 
 }
 int string_last_index(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int i= s .len - p .len ;
 
 while ( i >= 0 ) {
 
 
int j= 0 ;
 
 while ( j < p .len  &&  s .str[ i + j ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 
 j ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
return  i ;
 
 }
 ;
 
 i -- ;
 
 }
 ;
 
return  - 1 ;
 
 }
 int string_index_after(string s, string p, int start) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int strt= start ;
 
 if ( start < 0 ) {
 
 strt  =  0 ;
 
 }
 ;
 
 if ( start >= s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int i= strt ;
 
 while ( i < s .len ) {
 
 
int j= 0 ;
 
int ii= i ;
 
 while ( j < p .len  &&  s .str[ ii ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 
 j ++ ;
 
 ii ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
return  i ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  - 1 ;
 
 }
 int string_count(string s, string substr) {
 
 if ( s .len == 0  ||  substr .len == 0 ) {
 
return  0 ;
 
 }
 ;
 
 if ( substr .len > s .len ) {
 
return  0 ;
 
 }
 ;
 
int n= 0 ;
 
int i= 0 ;
 
 while (1) { 
 
 i  =  string_index_after( s , substr , i ) ;
 
 if ( i == - 1 ) {
 
return  n ;
 
 }
 ;
 
 i  +=  substr .len ;
 
 n ++ ;
 
 }
 ;
 
return  0 ;
 
 }
 bool string_contains(string s, string p) {
 
bool res= string_index( s , p ) > 0 - 1 ;
 
return  res ;
 
 }
 bool string_starts_with(string s, string p) {
 
bool res= string_index( s , p ) == 0 ;
 
return  res ;
 
 }
 bool string_ends_with(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  0 ;
 
 }
 ;
 
bool res= string_last_index( s , p ) == s .len - p .len ;
 
return  res ;
 
 }
 string string_to_lower(string s) {
 
byte* b= v_malloc ( s .len + 1 ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 b [/*ptr*/ i ]/*rbyte 1*/  =  tolower ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
 }
 ;
 
return  tos ( b , s .len ) ;
 
 }
 string string_to_upper(string s) {
 
byte* b= v_malloc ( s .len + 1 ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 b [/*ptr*/ i ]/*rbyte 1*/  =  toupper ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
 }
 ;
 
return  tos ( b , s .len ) ;
 
 }
 string string_capitalize(string s) {
 
string sl= string_to_lower( s ) ;
 
string cap=string_add( string_to_upper( byte_str( sl .str[ 0 ]/*rbyte 0*/ ) ) , string_right( sl , 1 ) ) ;
 
return  cap ;
 
 }
 string string_title(string s) {
 
array_string words= string_split( s , tos2((byte*)" ") ) ;
 
array_string tit=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 array_string tmp86 =  words;
 for (int tmp87 = 0; tmp87 < tmp86.len; tmp87++) {
 string word = ((string *) tmp86 . data)[tmp87];
 
 
_PUSH(& tit , ( string_capitalize( word ) ), tmp88, string) ;
 
 }
 ;
 
string title= array_string_join( tit , tos2((byte*)" ") ) ;
 
return  title ;
 
 }
 string string_find_between(string s, string start, string end) {
 
int start_pos= string_index( s , start ) ;
 
 if ( start_pos == - 1 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
string val= string_right( s , start_pos + start .len ) ;
 
int end_pos= string_index( val , end ) ;
 
 if ( end_pos == - 1 ) {
 
return  val ;
 
 }
 ;
 
return  string_left( val , end_pos ) ;
 
 }
 bool array_string_contains(array_string ar, string val) {
 
 array_string tmp93 =  ar;
 for (int tmp94 = 0; tmp94 < tmp93.len; tmp94++) {
 string s = ((string *) tmp93 . data)[tmp94];
 
 
 if (string_eq( s , val ) ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 }
 bool array_int_contains(array_int ar, int val) {
 
 array_int tmp95 =  ar;
 for (int i = 0; i < tmp95.len; i++) {
 int s = ((int *) tmp95 . data)[i];
 
 
 if ( s == val ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 }
 bool is_space(byte c) {
 
return _IN(byte, ( c ), new_array_from_c_array(6, 6, sizeof(byte), (byte[]) {  ' ' ,  '\n' ,  '\t' ,  '\v' ,  '\f' ,  '\r'  }) ) ;
 
 }
 bool byte_is_space(byte c) {
 
return  is_space ( c ) ;
 
 }
 string string_trim_space(string s) {
 
return  string_trim( s , tos2((byte*)" \n\t\v\f\r") ) ;
 
 }
 string string_trim(string s, string cutset) {
 
 if ( s .len < 1  ||  cutset .len < 1 ) {
 
return  s ;
 
 }
 ;
 
array_byte cs_arr= string_bytes( cutset ) ;
 
int pos_left= 0 ;
 
int pos_right= s .len - 1 ;
 
bool cs_match= 1 ;
 
 while ( pos_left <= s .len  &&  pos_right >= - 1  &&  cs_match ) {
 
 
 cs_match  =  0 ;
 
 if (_IN(byte, ( s .str[ pos_left ]/*rbyte 0*/ ),  cs_arr ) ) {
 
 pos_left ++ ;
 
 cs_match  =  1 ;
 
 }
 ;
 
 if (_IN(byte, ( s .str[ pos_right ]/*rbyte 0*/ ),  cs_arr ) ) {
 
 pos_right -- ;
 
 cs_match  =  1 ;
 
 }
 ;
 
 if ( pos_left > pos_right ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
 }
 ;
 
return  string_substr( s , pos_left , pos_right + 1 ) ;
 
 }
 string string_trim_left(string s, string cutset) {
 
 if ( s .len < 1  ||  cutset .len < 1 ) {
 
return  s ;
 
 }
 ;
 
array_byte cs_arr= string_bytes( cutset ) ;
 
int pos= 0 ;
 
 while ( pos <= s .len  && _IN(byte, ( s .str[ pos ]/*rbyte 0*/ ),  cs_arr ) ) {
 
 
 pos ++ ;
 
 }
 ;
 
return  string_right( s , pos ) ;
 
 }
 string string_trim_right(string s, string cutset) {
 
 if ( s .len < 1  ||  cutset .len < 1 ) {
 
return  s ;
 
 }
 ;
 
array_byte cs_arr= string_bytes( cutset ) ;
 
int pos= s .len - 1 ;
 
 while ( pos >= - 1  && _IN(byte, ( s .str[ pos ]/*rbyte 0*/ ),  cs_arr ) ) {
 
 
 pos -- ;
 
 }
 ;
 
return  string_left( s , pos + 1 ) ;
 
 }
 int compare_strings(string* a, string* b) {
 
 if ( string_lt(* a ,* b ) ) {
 
return  - 1 ;
 
 }
 ;
 
 if ( string_gt(* a ,* b ) ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 }
 int compare_strings_by_len(string* a, string* b) {
 
 if ( a ->len < b ->len ) {
 
return  - 1 ;
 
 }
 ;
 
 if ( a ->len > b ->len ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 }
 int compare_lower_strings(string* a, string* b) {
 
string aa= string_to_lower(* a ) ;
 
string bb= string_to_lower(* b ) ;
 
return  compare_strings (& /*112 EXP:"string*" GOT:"string" */ aa ,& /*112 EXP:"string*" GOT:"string" */ bb ) ;
 
 }
 void array_string_sort(array_string* s) {
 
 array_sort_with_compare( s , compare_strings ) ;
 
 }
 void array_string_sort_ignore_case(array_string* s) {
 
 array_sort_with_compare( s , compare_lower_strings ) ;
 
 }
 void array_string_sort_by_len(array_string* s) {
 
 array_sort_with_compare( s , compare_strings_by_len ) ;
 
 }
 ustring string_ustring(string s) {
 
ustring res= (ustring) { .s =  s , .runes =  new_array ( 0 , s .len , sizeof( int) ) , .len =  0 } ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
int char_len= utf8_char_len ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
_PUSH(& res .runes , ( i ), tmp109, int) ;
 
 i  +=  char_len - 1 ;
 
 res .len ++ ;
 
 }
 ;
 
return  res ;
 
 }
 ustring string_ustring_tmp(string s) {
 
 if ( g_ustring_runes .len == 0 ) {
 
 g_ustring_runes  =  new_array ( 0 , 128 , sizeof( int) ) ;
 
 }
 ;
 
ustring res= (ustring) { .s =  s , .runes =  new_array(0, 1, sizeof( int )) , .len =  0 } ;
 
 res .runes  =  g_ustring_runes ;
 
 res .runes .len  =  s .len ;
 
int j= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
int char_len= utf8_char_len ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
array_set(&/*q*/ res .runes , j , & (int []) {  i }) ;
 
 j ++ ;
 
 i  +=  char_len - 1 ;
 
 res .len ++ ;
 
 }
 ;
 
return  res ;
 
 }
 string ustring_substr(ustring u, int _start, int _end) {
 
int start= ( *(int*) array__get( u .runes , _start) ) ;
 
int end= ( _end >= u .runes .len ) ? ( u .s .len ) : ( ( *(int*) array__get( u .runes , _end) ) ) ;
 
return  string_substr( u .s , start , end ) ;
 
 }
 string ustring_left(ustring u, int pos) {
 
return  ustring_substr( u , 0 , pos ) ;
 
 }
 string ustring_right(ustring u, int pos) {
 
return  ustring_substr( u , pos , u .len ) ;
 
 }
 byte string_at(string s, int idx) {
 
 if ( idx < 0  ||  idx >= s .len ) {
 
 v_panic ( _STR("string index out of range: %d / %d", idx, s .len) ) ;
 
 }
 ;
 
return  s .str [/*ptr*/ idx ]/*rbyte 0*/ ;
 
 }
 string ustring_at(ustring u, int idx) {
 
return  ustring_substr( u , idx , idx + 1 ) ;
 
 }
 void v_ustring_free(ustring u) {
 
 v_array_free( u .runes ) ;
 
 }
 bool byte_is_digit(byte c) {
 
return  c >= '0'  &&  c <= '9' ;
 
 }
 bool byte_is_hex_digit(byte c) {
 
return  byte_is_digit( c )  ||  ( c >= 'a'  &&  c <= 'f' )  ||  ( c >= 'A'  &&  c <= 'F' ) ;
 
 }
 bool byte_is_oct_digit(byte c) {
 
return  c >= '0'  &&  c <= '7' ;
 
 }
 bool byte_is_letter(byte c) {
 
return  ( c >= 'a'  &&  c <= 'z' )  ||  ( c >= 'A'  &&  c <= 'Z' ) ;
 
 }
 void v_string_free(string s) {
 
 v_free ( s .str ) ;
 
 }
 string string_all_before(string s, string dot) {
 
int pos= string_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_left( s , pos ) ;
 
 }
 string string_all_before_last(string s, string dot) {
 
int pos= string_last_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_left( s , pos ) ;
 
 }
 string string_all_after(string s, string dot) {
 
int pos= string_last_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_right( s , pos + dot .len ) ;
 
 }
 string array_string_join(array_string a, string del) {
 
 if ( a .len == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
int len= 0 ;
 
 array_string tmp124 =  a;
 for (int i = 0; i < tmp124.len; i++) {
 string val = ((string *) tmp124 . data)[i];
 
 
 len  +=  val .len + del .len ;
 
 }
 ;
 
 len  -=  del .len ;
 
string res= tos2((byte*)"") ;
 
 res .len  =  len ;
 
 res .str  =  v_malloc ( res .len + 1 ) ;
 
int idx= 0 ;
 
 array_string tmp127 =  a;
 for (int i = 0; i < tmp127.len; i++) {
 string val = ((string *) tmp127 . data)[i];
 
 
 for (
int j= 0  ;  j < val .len  ;  j ++ ) { 
 
 
byte c= val .str[ j ]/*rbyte 0*/ ;
 
 res .str [/*ptr*/ idx ]/*rbyte 1*/  =  val .str [/*ptr*/ j ]/*rbyte 0*/ ;
 
 idx ++ ;
 
 }
 ;
 
 if ( i != a .len - 1 ) {
 
 for (
int k= 0  ;  k < del .len  ;  k ++ ) { 
 
 
 res .str [/*ptr*/ idx ]/*rbyte 1*/  =  del .str [/*ptr*/ k ]/*rbyte 0*/ ;
 
 idx ++ ;
 
 }
 ;
 
 }
 ;
 
 }
 ;
 
 res .str [/*ptr*/ res .len ]/*rbyte 1*/  =  '\0' ;
 
return  res ;
 
 }
 string array_string_join_lines(array_string s) {
 
return  array_string_join( s , tos2((byte*)"\n") ) ;
 
 }
 string string_reverse(string s) {
 
string res= (string) { .len =  s .len , .str =  v_malloc ( s .len ) } ;
 
 for (
int i= s .len - 1  ;  i >= 0  ;  i -- ) { 
 
 
 res .str[ s .len - i - 1 ]/*rbyte 1*/  =  s .str[ i ]/*rbyte 0*/ ;
 
 }
 ;
 
return  res ;
 
 }
 string string_limit(string s, int max) {
 
ustring u= string_ustring( s ) ;
 
 if ( u .len <= max ) {
 
return  s ;
 
 }
 ;
 
return  ustring_substr( u , 0 , max ) ;
 
 }
 bool byte_is_white(byte c) {
 
int i= ((int)( c ) ) ;
 
return  i == 10  ||  i == 32  ||  i == 9  ||  i == 13  ||  c == '\r' ;
 
 }
 int string_hash(string s) {
 
int h= 0 ;
 
 if ( h == 0  &&  s .len > 0 ) {
 
 string tmp136 =  s;
 array_byte bytes_tmp136 = string_bytes( tmp136 );
 ;
for (int tmp137 = 0; tmp137 < tmp136 .len; tmp137 ++) {
 byte c = ((byte *) bytes_tmp136 . data)[tmp137];
 
 
 h  =  h * 31 + ((int)( c ) ) ;
 
 }
 ;
 
 }
 ;
 
return  h ;
 
 }
 array_byte string_bytes(string s) {
 
 if ( s .len == 0 ) {
 
return new_array_from_c_array(0, 0, sizeof(byte), (byte[]) {   0 }) ;
 
 }
 ;
 
array_byte buf= array_repeat(new_array_from_c_array(1, 1, sizeof(byte), (byte[]) {  ((byte)( 0 ) )  }) , s .len ) ;
 
 memcpy ( buf .data , (char*) s .str ,  s .len ) ;
 
return  buf ;
 
 }
 void v_exit(int code) {
 
 exit ( code ) ;
 
 }
 bool isnil(void* v) {
 
return  v == 0 ;
 
 }
 void on_panic(int (*f)( int  /*FFF*/ )) {
 
 }
 void print_backtrace_skipping_top_frames(int skipframes) {
 
 #ifdef __APPLE__
 
byte* buffer  [100 ] ;
 
int nr_ptrs= backtrace ( ((voidptr*)( buffer ) ) ,  100 ) ;
 
 backtrace_symbols_fd ( ((voidptr*)( & /*v*/  buffer [ skipframes ]/*rbyte* 0*/ ) ) ,  nr_ptrs - skipframes ,  1 ) ;
 
 return ;
 
 #endif
 ;
 
 #ifdef __linux__
 
 #ifndef __BIONIC__
 
 if ( backtrace_symbols_fd != 0 ) {
 
byte* buffer  [100 ] ;
 
int nr_ptrs= backtrace ( ((voidptr*)( buffer ) ) ,  100 ) ;
 
 backtrace_symbols_fd ( & /*v*/  buffer [ skipframes ]/*rbyte* 0*/ ,  nr_ptrs - skipframes ,  1 ) ;
 
 return ;
 
 }
  else { 
 
 printf ( "backtrace_symbols_fd is missing, so printing backtraces is not available.\n" ) ;
 
 printf ( "Some libc implementations like musl simply do not provide it.\n" ) ;
 
 }
 ;
 
 #endif
 ;
 
 #endif
 ;
 
 println ( tos2((byte*)"print_backtrace_skipping_top_frames is not implemented on this platform for now...\n") ) ;
 
 }
 void print_backtrace() {
 
 print_backtrace_skipping_top_frames ( 2 ) ;
 
 }
 void _panic_debug(int line_no, string file, string mod, string fn_name, string s) {
 
 println ( tos2((byte*)"================ V panic ================") ) ;
 
printf( "   module: %.*s\n", mod.len, mod.str ) ;
 
printf( " function: %.*s()\n", fn_name.len, fn_name.str ) ;
 
printf( "     file: %.*s\n", file.len, file.str ) ;
 
 println (string_add( tos2((byte*)"     line: ") , int_str( line_no ) ) ) ;
 
printf( "  message: %.*s\n", s.len, s.str ) ;
 
 println ( tos2((byte*)"=========================================") ) ;
 
 print_backtrace_skipping_top_frames ( 1 ) ;
 
 exit ( 1 ) ;
 
 }
 void v_panic(string s) {
 
printf( "V panic: %.*s\n", s.len, s.str ) ;
 
 print_backtrace ( ) ;
 
 exit ( 1 ) ;
 
 }
 void println(string s) {
 
 if ( isnil ( s .str ) ) {
 
 v_panic ( tos2((byte*)"println(NIL)") ) ;
 
 }
 ;
 
 #ifdef _WIN32
 
 _putws ( string_to_wide( s ) ) ;
 
 #else
 
 printf ( "%.*s\n" ,  s .len , (char*) s .str ) ;
 
 #endif
 ;
 
 }
 void eprintln(string s) {
 
 if ( isnil ( s .str ) ) {
 
 v_panic ( tos2((byte*)"eprintln(NIL)") ) ;
 
 }
 ;
 
 #ifdef __APPLE__
 
 fprintf ( stderr ,  "%.*s\n" ,  s .len , (char*) s .str ) ;
 
 #else
 
 println ( s ) ;
 
 #endif
 ;
 
 }
 void print(string s) {
 
 #ifdef _WIN32
 
 wprintf ( string_to_wide( s ) ) ;
 
 #else
 
 printf ( "%.*s" ,  s .len , (char*) s .str ) ;
 
 #endif
 ;
 
 }
 byte* v_malloc(int n) {
 
 if ( n < 0 ) {
 
 v_panic ( tos2((byte*)"malloc(<0)") ) ;
 
 }
 ;
 
byte* ptr= malloc ( n ) ;
 
 if ( isnil ( ptr ) ) {
 
 v_panic ( _STR("malloc(%d) failed", n) ) ;
 
 }
 ;
 
return  ptr ;
 
 }
 byte* v_calloc(int n) {
 
 if ( n < 0 ) {
 
 v_panic ( tos2((byte*)"calloc(<0)") ) ;
 
 }
 ;
 
return  calloc ( n ,  1 ) ;
 
 }
 void v_free(void* ptr) {
 
 free ( ptr ) ;
 
 }
 void* memdup(void* src, int sz) {
 
byte* mem= v_malloc ( sz ) ;
 
return  memcpy ((char*) mem ,  src ,  sz ) ;
 
 }
 void v_ptr_free(void* ptr) {
 
 free ( ptr ) ;
 
 }
 string f64_str(f64 d) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( ((char*)( buf ) ) ,  "%f" ,  d ) ;
 
return  tos ( buf , vstrlen ( buf ) ) ;
 
 }
 string f32_str(f32 d) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( ((char*)( buf ) ) ,  "%f" ,  d ) ;
 
return  tos ( buf , vstrlen ( buf ) ) ;
 
 }
 string ptr_str(void* ptr) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( ((char*)( buf ) ) ,  "%p" ,  ptr ) ;
 
return  tos ( buf , vstrlen ( buf ) ) ;
 
 }
 bool f64_eq(f64 a, f64 b) {
 
return  ( a - b ) <= DBL_EPSILON ;
 
 }
 string int_str(int nn) {
 
int n= nn ;
 
 if ( n == 0 ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 16 ;
 
byte* buf= v_calloc ( max ) ;
 
int len= 0 ;
 
bool is_neg= 0 ;
 
 if ( n < 0 ) {
 
 n  =  - n ;
 
 is_neg  =  1 ;
 
 }
 ;
 
 while ( n > 0 ) {
 
 
int d= n % 10 ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((int)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / 10 ;
 
 }
 ;
 
 if ( is_neg ) {
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 
 len ++ ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 }
 string u32_str(u32 nn) {
 
u32 n= nn ;
 
 if ( n == ((u32)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 16 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
 while ( n > ((u32)( 0 ) ) ) {
 
 
u32 d= n % ((u32)( 10 ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u32)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((u32)( 10 ) ) ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 }
 string i64_str(i64 nn) {
 
i64 n= nn ;
 
 if ( n == ((i64)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 32 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
bool is_neg= 0 ;
 
 if ( n < ((i64)( 0 ) ) ) {
 
 n  =  - n ;
 
 is_neg  =  1 ;
 
 }
 ;
 
 while ( n > ((i64)( 0 ) ) ) {
 
 
int d= ((int)( n % ((i64)( 10 ) ) ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((int)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((i64)( 10 ) ) ;
 
 }
 ;
 
 if ( is_neg ) {
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 
 len ++ ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 }
 string u64_str(u64 nn) {
 
u64 n= nn ;
 
 if ( n == ((u64)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 32 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
 while ( n > ((u64)( 0 ) ) ) {
 
 
u64 d= n % ((u64)( 10 ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u64)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((u64)( 10 ) ) ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 }
 string bool_str(bool b) {
 
 if ( b ) {
 
return  tos2((byte*)"true") ;
 
 }
 ;
 
return  tos2((byte*)"false") ;
 
 }
 string int_hex(int n) {
 
int len= ( n >= 0 ) ? ( int_str( n ) .len + 3 ) : ( 11 ) ;
 
byte* hex= v_malloc ( len ) ;
 
int count= ((int)( sprintf ( ((char*)( hex ) ) ,  "0x%x" ,  n ) ) ) ;
 
return  tos ( hex , count ) ;
 
 }
 string i64_hex(i64 n) {
 
int len= ( n >= ((i64)( 0 ) ) ) ? ( i64_str( n ) .len + 3 ) : ( 19 ) ;
 
byte* hex= v_malloc ( len ) ;
 
int count= ((int)( sprintf ( ((char*)( hex ) ) ,  "0x%llx" ,  n ) ) ) ;
 
return  tos ( hex , count ) ;
 
 }
 bool array_byte_contains(array_byte a, byte val) {
 
 array_byte tmp32 =  a;
 for (int tmp33 = 0; tmp33 < tmp32.len; tmp33++) {
 byte aa = ((byte *) tmp32 . data)[tmp33];
 
 
 if ( aa == val ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 }
 string rune_str(rune c) {
 
int fst_byte= ((int)( c ) )  >>  8 * 3 & 0xff ;
 
int len= utf8_char_len ( fst_byte ) ;
 
string str= (string) { .len =  len , .str =  v_malloc ( len + 1 ) } ;
 
 for (
int i= 0  ;  i < len  ;  i ++ ) { 
 
 
 str .str [/*ptr*/ i ]/*rbyte 1*/  =  ((int)( c ) )  >>  8 * ( 3 - i ) & 0xff ;
 
 }
 ;
 
 str .str[ len ]/*rbyte 1*/  =  '\0' ;
 
return  str ;
 
 }
 string byte_str(byte c) {
 
string str= (string) { .len =  1 , .str =  v_malloc ( 2 ) } ;
 
 str .str [/*ptr*/ 0 ]/*rbyte 1*/  =  c ;
 
 str .str [/*ptr*/ 1 ]/*rbyte 1*/  =  '\0' ;
 
return  str ;
 
 }
 bool byte_is_capital(byte c) {
 
return  c >= 'A'  &&  c <= 'Z' ;
 
 }
 array_byte array_byte_clone(array_byte b) {
 
array_byte res= array_repeat(new_array_from_c_array(1, 1, sizeof(byte), (byte[]) {  ((byte)( 0 ) )  }) , b .len ) ;
 
 for (
int i= 0  ;  i < b .len  ;  i ++ ) { 
 
 
array_set(&/*q*/ res , i , & (byte []) {  ( *(byte*) array__get( b , i) ) }) ;
 
 }
 ;
 
return  res ;
 
 }
 int utf8_char_len(byte b) {
 
return  ( ( 0xe5000000  >>  ( ( b  >>  3 ) & 0x1e ) ) & 3 ) + 1 ;
 
 }
 string utf32_to_str(u32 code) {
 
int icode= ((int)( code ) ) ;
 
byte* buffer= v_malloc ( 5 ) ;
 
 if ( icode <= 127 ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  icode ;
 
return  tos ( buffer , 1 ) ;
 
 }
 ;
 
 if ( ( icode <= 2047 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  192 | ( icode  >>  6 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 2 ) ;
 
 }
 ;
 
 if ( ( icode <= 65535 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  224 | ( icode  >>  12 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyte 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 3 ) ;
 
 }
 ;
 
 if ( ( icode <= 1114111 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  240 | ( icode  >>  18 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyte 1*/  =  128 | ( ( icode  >>  12 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyte 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 3 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 4 ) ;
 
 }
 ;
 
return  tos2((byte*)"") ;
 
 }
 string utf32_to_str_no_malloc(u32 code, void* buf) {
 
int icode= ((int)( code ) ) ;
 
byteptr buffer= ((byteptr)( buf ) ) ;
 
 if ( icode <= 127 ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  icode ;
 
return  tos ( buffer , 1 ) ;
 
 }
 ;
 
 if ( ( icode <= 2047 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  192 | ( icode  >>  6 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 2 ) ;
 
 }
 ;
 
 if ( ( icode <= 65535 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  224 | ( icode  >>  12 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 3 ) ;
 
 }
 ;
 
 if ( ( icode <= 1114111 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  240 | ( icode  >>  18 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  12 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 3 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 4 ) ;
 
 }
 ;
 
return  tos2((byte*)"") ;
 
 }
 int string_utf32_code(string _rune) {
 
 if ( _rune .len == 0 ) {
 
return  0 ;
 
 }
 ;
 
 if ( _rune .len == 1 ) {
 
return  ((int)( _rune .str[ 0 ]/*rbyte 0*/ ) ) ;
 
 }
 ;
 
byte b= ((byte)( ((int)( _rune .str[ 0 ]/*rbyte 0*/ ) ) ) ) ;
 
 b  =  b  <<  _rune .len ;
 
int res= ((int)( b ) ) ;
 
int shift= 6 - _rune .len ;
 
 for (
int i= 1  ;  i < _rune .len  ;  i ++ ) { 
 
 
int c= ((int)( _rune .str[ i ]/*rbyte 0*/ ) ) ;
 
 res  =  res  <<  shift ;
 
 res  |=  c & 63 ;
 
 shift  =  6 ;
 
 }
 ;
 
return  res ;
 
 }
 u16* string_to_wide(string _str) {
 
 #ifdef _WIN32
 
int num_chars= ((int)( MultiByteToWideChar ( builtin__CP_UTF8 ,  0 , (char*) _str .str ,  _str .len ,  0 ,  0 ) ) ) ;
 
u16* wstr= ((u16*)( v_malloc ( ( num_chars + 1 ) * 2 ) ) ) ;
 
 if ( wstr > 0 ) {
 
 MultiByteToWideChar ( builtin__CP_UTF8 ,  0 , (char*) _str .str ,  _str .len ,  wstr ,  num_chars ) ;
 
 memset ((char*) ((byte*)( wstr ) ) + num_chars * 2 ,  0 ,  2 ) ;
 
 }
 ;
 
return  wstr ;
 
 #else
 
return  0 ;
 
 #endif
 ;
 
 }
 string string_from_wide(u16* _wstr) {
 
 #ifdef _WIN32
 
int wstr_len= ((int)( wcslen ( _wstr ) ) ) ;
 
return  string_from_wide2 ( _wstr , wstr_len ) ;
 
 #else
 
return  tos2((byte*)"") ;
 
 #endif
 ;
 
 }
 string string_from_wide2(u16* _wstr, int len) {
 
 #ifdef _WIN32
 
int num_chars= ((int)( WideCharToMultiByte ( builtin__CP_UTF8 ,  0 ,  _wstr ,  len ,  0 ,  0 ,  0 ,  0 ) ) ) ;
 
byte* str_to= ((byte*)( v_malloc ( num_chars + 1 ) ) ) ;
 
 if ( str_to > 0 ) {
 
 WideCharToMultiByte ( builtin__CP_UTF8 ,  0 ,  _wstr ,  len , (char*) str_to ,  num_chars ,  0 ,  0 ) ;
 
 memset ((char*) ((byte*)( str_to ) ) + num_chars ,  0 ,  1 ) ;
 
 }
 ;
 
return  tos2 ( str_to ) ;
 
 #else
 
return  tos2((byte*)"") ;
 
 #endif
 ;
 
 }
 int utf8_len(byte c) {
 
int b= 0 ;
 
byte x= c ;
 
 if ( ( ( x & 240 ) != 0 ) ) {
 
 x  >>=  4 ;
 
 }
  else { 
 
 b  +=  4 ;
 
 }
 ;
 
 if ( ( ( x & 12 ) != 0 ) ) {
 
 x  >>=  2 ;
 
 }
  else { 
 
 b  +=  2 ;
 
 }
 ;
 
 if ( ( ( x & 2 ) == 0 ) ) {
 
 b ++ ;
 
 }
 ;
 
return  b ;
 
 }
 int utf8_getchar() {
 
int c= ((int)( getchar ( ) ) ) ;
 
int len= utf8_len ( ~ c ) ;
 
 if ( c < 0 ) {
 
return  0 ;
 
 }
  else  if ( len == 0 ) {
 
return  c ;
 
 }
  else  if ( len == 1 ) {
 
return  - 1 ;
 
 }
  else { 
 
int uc= ((int)( c & ( ( 1  <<  ( 7 - len ) ) - 1 ) ) ) ;
 
 for (
int i= 0  ;  i + 1 < len  ;  i ++ ) { 
 
 
int c2= ((int)( getchar ( ) ) ) ;
 
 if ( c2 != - 1  &&  ( c2  >>  6 ) == 2 ) {
 
 uc  <<=  6 ;
 
 uc  |=  ((int)( ( c2 & 63 ) ) ) ;
 
 }
  else  if ( c2 == - 1 ) {
 
return  0 ;
 
 }
  else { 
 
return  - 1 ;
 
 }
 ;
 
 }
 ;
 
return  uc ;
 
 }
 ;
 
 }
 map new_map(int cap, int elm_size) {
 
map res= (map) { .element_size =  elm_size , .root =  0 , .size =  0 } ;
 
return  res ;
 
 }
 map new_map_init(int cap, int elm_size, string* keys, void* vals) {
 
map res= (map) { .element_size =  elm_size , .root =  0 , .size =  0 } ;
 
 int tmp3 =  0;
 ;
for (int tmp4 = tmp3; tmp4 <  cap; tmp4++) {
 int i = tmp4;
 
 
 map__set(& /* ? */ res , keys [/*ptr*/ i ]/*rstring 0*/ ,(byte*) vals + i * elm_size ) ;
 
 }
 ;
 
return  res ;
 
 }
 mapnode* new_node(string key, void* val, int element_size) {
 
mapnode* new_e= (mapnode*)memdup(&(mapnode)  { .key =  key , .val =  v_malloc ( element_size ) , .left =  0 , .right =  0 , .is_empty =  0 , } , sizeof(mapnode)) ;
 
 memcpy ( new_e ->val ,  val ,  element_size ) ;
 
return  new_e ;
 
 }
 void map_insert(map* m, mapnode* n, string key, void* val) {
 
 if (string_eq( n ->key , key ) ) {
 
 memcpy ( n ->val ,  val ,  m ->element_size ) ;
 
 return ;
 
 }
 ;
 
 if (string_gt( n ->key , key ) ) {
 
 if ( n ->left == 0 ) {
 
 n ->left  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 }
  else { 
 
 map_insert( m , n ->left , key , val ) ;
 
 }
 ;
 
 return ;
 
 }
 ;
 
 if ( n ->right == 0 ) {
 
 n ->right  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 }
  else { 
 
 map_insert( m , n ->right , key , val ) ;
 
 }
 ;
 
 }
 bool mapnode_find(mapnode* n, string key, void* out, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
 memcpy ( out ,  n ->val ,  element_size ) ;
 
return  1 ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( n ->left == 0 ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find(& /* ? */* n ->left , key , out , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( n ->right == 0 ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find(& /* ? */* n ->right , key , out , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 }
 bool mapnode_find2(mapnode* n, string key, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
return  1 ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find2(& /* ? */* n ->left , key , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( isnil ( n ->right ) ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find2(& /* ? */* n ->right , key , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 }
 void map__set(map* m, string key, void* val) {
 
 if ( isnil ( m ->root ) ) {
 
 m ->root  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 return ;
 
 }
 ;
 
 map_insert( m , m ->root , key , val ) ;
 
 }
 int preorder_keys(mapnode* node, array_string* keys, int key_i) {
 
int i= key_i ;
 
 if ( ! node ->is_empty ) {
 
array_set( keys , i , & (string []) {  node ->key }) ;
 
 i ++ ;
 
 }
 ;
 
 if ( ! isnil ( node ->left ) ) {
 
 i  =  preorder_keys ( node ->left , keys , i ) ;
 
 }
 ;
 
 if ( ! isnil ( node ->right ) ) {
 
 i  =  preorder_keys ( node ->right , keys , i ) ;
 
 }
 ;
 
return  i ;
 
 }
 array_string map_keys(map* m) {
 
array_string keys= array_repeat(new_array_from_c_array(1, 1, sizeof(string), (string[]) {  tos2((byte*)"")  }) , m ->size ) ;
 
 if ( isnil ( m ->root ) ) {
 
return  keys ;
 
 }
 ;
 
 preorder_keys ( m ->root ,& /*111*/ (array[]){ keys }[0]  , 0 ) ;
 
return  keys ;
 
 }
 bool map_get(map m, string key, void* out) {
 
 if ( m .root == 0 ) {
 
return  0 ;
 
 }
 ;
 
return  mapnode_find(& /* ? */* m .root , key , out , m .element_size ) ;
 
 }
 void v_mapnode_delete(mapnode* n, string key, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
 memset ( n ->val ,  0 ,  element_size ) ;
 
 n ->is_empty  =  1 ;
 
 return ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
 return ;
 
 }
  else { 
 
 v_mapnode_delete( n ->left , key , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( isnil ( n ->right ) ) {
 
 return ;
 
 }
  else { 
 
 v_mapnode_delete( n ->right , key , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 }
 void v_map_delete(map* m, string key) {
 
 v_mapnode_delete( m ->root , key , m ->element_size ) ;
 
 m ->size -- ;
 
 }
 void map_exists(map m, string key) {
 
 v_panic ( tos2((byte*)"map.exists(key) was removed from the language. Use `key in map` instead.") ) ;
 
 }
 bool map__exists(map m, string key) {
 
return  ! isnil ( m .root )  &&  mapnode_find2(& /* ? */* m .root , key , m .element_size ) ;
 
 }
 void map_print(map m) {
 
 println ( tos2((byte*)"<<<<<<<<") ) ;
 
 println ( tos2((byte*)">>>>>>>>>>") ) ;
 
 }
 void v_mapnode_free(mapnode* n) {
 
 if ( n ->val != 0 ) {
 
 v_free ( n ->val ) ;
 
 }
 ;
 
 if ( n ->left != 0 ) {
 
 v_mapnode_free( n ->left ) ;
 
 }
 ;
 
 if ( n ->right != 0 ) {
 
 v_mapnode_free( n ->right ) ;
 
 }
 ;
 
 v_free ( n ) ;
 
 }
 void v_map_free(map* m) {
 
 if ( m ->root == 0 ) {
 
 return ;
 
 }
 ;
 
 v_mapnode_free( m ->root ) ;
 
 }
 string map_string_str(map_string m) {
 
 if ( m .size == 0 ) {
 
return  tos2((byte*)"{}") ;
 
 }
 ;
 
strings__Builder sb= strings__new_builder ( 50 ) ;
 
 strings__Builder_writeln(& /* ? */ sb , tos2((byte*)"{") ) ;
 
 map_string tmp9 =  m;
 array_string keys_tmp9 = map_keys(& tmp9 ); 
 for (int l = 0; l < keys_tmp9 .len; l++) {
 string key = ((string*)keys_tmp9 .data)[l];
 string val = {0}; map_get(tmp9, key, & val);
 
 
 strings__Builder_writeln(& /* ? */ sb , _STR("  \"%.*s\" => \"%.*s\"", key.len, key.str, val.len, val.str) ) ;
 
 }
 ;
 
 strings__Builder_writeln(& /* ? */ sb , tos2((byte*)"}") ) ;
 
return  strings__Builder_str( sb ) ;
 
 }
 Option opt_ok(void* data, int size) {
 
 if ( size >= 255 ) {
 
 v_panic ( _STR("option size too big: %d (max is 255), this is a temporary limit", size) ) ;
 
 }
 ;
 
Option res= (Option) { .ok =  1 , .error =  tos((byte *)"", 0) , .is_none =  0 } ;
 
 memcpy ( res .data ,  data ,  size ) ;
 
return  res ;
 
 }
 Option opt_none() {
 
return  (Option) { .is_none =  1 , .error =  tos((byte *)"", 0) , .ok =  0 , } ;
 
 }
 Option v_error(string s) {
 
return  (Option) { .error =  s , .ok =  0 , .is_none =  0 } ;
 
 }
 strings__Builder strings__new_builder(int initial_size) {
 
return  (strings__Builder) { .buf =  new_array(0, 1, sizeof( byte )) , .len =  0 } ;
 
 }
 void strings__Builder_write(strings__Builder* b, string s) {
 
 array__push_many(& /* ? */ b ->buf , s .str , s .len ) ;
 
 b ->len  +=  s .len ;
 
 }
 void strings__Builder_writeln(strings__Builder* b, string s) {
 
 array__push_many(& /* ? */ b ->buf , s .str , s .len ) ;
 
_PUSH(& b ->buf , ( '\n' ), tmp1, byte) ;
 
 b ->len  +=  s .len + 1 ;
 
 }
 string strings__Builder_str(strings__Builder b) {
 
return  (tos((byte *) b .buf .data ,  b .len ) ) ;
 
 }
 void strings__Builder_cut(strings__Builder* b, int n) {
 
 b ->len  -=  n ;
 
 }
 void strings__Builder_free(strings__Builder* b) {
 
 v_free ( b ->buf .data ) ;
 
 }
 int strings__levenshtein_distance(string a, string b) {
 
array_int f= array_repeat(new_array_from_c_array(1, 1, sizeof(int), (int[]) {  0  }) , b .len + 1 ) ;
 
 string tmp2 =  a;
 array_byte bytes_tmp2 = string_bytes( tmp2 );
 ;
for (int tmp3 = 0; tmp3 < tmp2 .len; tmp3 ++) {
 byte ca = ((byte *) bytes_tmp2 . data)[tmp3];
 
 
int j= 1 ;
 
int fj1= ( *(int*) array__get( f , 0) ) ;
 
 ( *(int*) array__get( f , 0) ) ++ ;
 
 string tmp10 =  b;
 array_byte bytes_tmp10 = string_bytes( tmp10 );
 ;
for (int tmp11 = 0; tmp11 < tmp10 .len; tmp11 ++) {
 byte cb = ((byte *) bytes_tmp10 . data)[tmp11];
 
 
int mn= ( ( *(int*) array__get( f , j) ) + 1 <= ( *(int*) array__get( f , j - 1) ) + 1 ) ? ( ( *(int*) array__get( f , j) ) + 1 ) : ( ( *(int*) array__get( f , j - 1) ) + 1 ) ;
 
 if ( cb != ca ) {
 
 mn  =  ( mn <= fj1 + 1 ) ? ( mn ) : ( fj1 + 1 ) ;
 
 }
  else { 
 
 mn  =  ( mn <= fj1 ) ? ( mn ) : ( fj1 ) ;
 
 }
 ;
 
 fj1  =  ( *(int*) array__get( f , j) ) ;
 
array_set(&/*q*/ f , j , & (int []) {  mn }) ;
 
 j ++ ;
 
 }
 ;
 
 }
 ;
 
return  ( *(int*) array__get( f , f .len - 1) ) ;
 
 }
 f32 strings__levenshtein_distance_percentage(string a, string b) {
 
int d= strings__levenshtein_distance ( a , b ) ;
 
int l= ( a .len >= b .len ) ? ( a .len ) : ( b .len ) ;
 
return  ( 1.00 - ((f32)( d ) ) / ((f32)( l ) ) ) * 100.00 ;
 
 }
 f32 strings__dice_coefficient(string s1, string s2) {
 
 if ( s1 .len == 0  ||  s2 .len == 0 ) {
 
return  0.0 ;
 
 }
 ;
 
 if (string_eq( s1 , s2 ) ) {
 
return  1.0 ;
 
 }
 ;
 
 if ( s1 .len < 2  ||  s2 .len < 2 ) {
 
return  0.0 ;
 
 }
 ;
 
string a= ( s1 .len > s2 .len ) ? ( s1 ) : ( s2 ) ;
 
string b= (string_eq( a , s1 ) ) ? ( s2 ) : ( s1 ) ;
 
map_int first_bigrams= new_map(1, sizeof(int)) ;
 
 for (
int i= 0  ;  i < a .len - 1  ;  i ++ ) { 
 
 
string bigram= string_substr( a , i , i + 2 ) ;
  
 int tmp32 = 0; bool tmp33 = map_get( first_bigrams , bigram, & tmp32); 

map__set(& first_bigrams , bigram , & (int []) {  (_IN_MAP( ( bigram ),  first_bigrams ) ) ? ( tmp32 + 1 ) : ( 1 ) }) ;
 
 }
 ;
 
int intersection_size= 0 ;
 
 for (
int i= 0  ;  i < b .len - 1  ;  i ++ ) { 
 
 
string bigram= string_substr( b , i , i + 2 ) ;
  
 int tmp37 = 0; bool tmp38 = map_get( first_bigrams , bigram, & tmp37); 

int count= (_IN_MAP( ( bigram ),  first_bigrams ) ) ? ( tmp37 ) : ( 0 ) ;
 
 if ( count > 0 ) {
 
map__set(& first_bigrams , bigram , & (int []) {  count - 1 }) ;
 
 intersection_size ++ ;
 
 }
 ;
 
 }
 ;
 
return  ( 2.0 * intersection_size ) / ( ((f32)( a .len ) ) + ((f32)( b .len ) ) - 2 ) ;
 
 }
 string strings__repeat(byte c, int n) {
 
 if ( n <= 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
array_byte arr= array_repeat(new_array_from_c_array(1, 1, sizeof(byte), (byte[]) {  ((byte)( 0 ) )  }) , n + 1 ) ;
 
 for (
int i= 0  ;  i < n  ;  i ++ ) { 
 
 
array_set(&/*q*/ arr , i , & (byte []) {  c }) ;
 
 }
 ;
 
array_set(&/*q*/ arr , n , & (byte []) {  '\0' }) ;
 
return  (tos((byte *) arr .data ,  n ) ) ;
 
 }
 Option_string net__hostname() {
 
byte name  [256 ] ;
 
void* res= gethostname ( & /*v*/  name ,  256 ) ;
 
 if ( res != 0 ) {
 
return  v_error ( tos2((byte*)"net.hostname() cannot get the host name") ) ;
 
 }
 ;
 
string tmp3 = OPTION_CAST(string)( tos_clone ( name )); return opt_ok(&tmp3, sizeof(string)) ;
 
 }
 Option_net__Socket net__socket(int family, int _type, int proto) {
 
 #ifdef _WIN32
  
 struct /*c struct init*/ 

WSAData wsadata ;
 
void* res= WSAStartup ( net__WSA_V22 ,  & /*v*/  wsadata ) ;
 
 if ( res != 0 ) {
 
return  v_error ( tos2((byte*)"socket: WSAStartup failed") ) ;
 
 }
 ;
 
 #endif
 ;
 
void* sockfd= socket ( family ,  _type ,  proto ) ;
 
int one= 1 ;
 
 setsockopt ( sockfd ,  SOL_SOCKET ,  SO_REUSEADDR ,  & /*v*/  one ,  sizeof( int) ) ;
 
 if ( sockfd == 0 ) {
 
return  v_error ( tos2((byte*)"socket: init failed") ) ;
 
 }
 ;
 
net__Socket s= (net__Socket) { .sockfd =  sockfd , .family =  family , ._type =  _type , .proto =  proto } ;
 
net__Socket tmp6 = OPTION_CAST(net__Socket)( s); return opt_ok(&tmp6, sizeof(net__Socket)) ;
 
 }
 Option_net__Socket net__socket_udp() {
 
Option_net__Socket tmp7 = OPTION_CAST(Option_net__Socket)( net__socket ( AF_INET , SOCK_DGRAM , IPPROTO_UDP )); return opt_ok(&tmp7, sizeof(net__Socket)) ;
 
 }
 Option_int net__Socket_setsockopt(net__Socket s, int level, int optname, int* optvalue) {
 
void* res= setsockopt ( s .sockfd ,  level ,  optname ,  optvalue ,   ( optvalue ) ) ;
 
 if ( res < 0 ) {
 
return  v_error ( tos2((byte*)"socket: setsockopt failed") ) ;
 
 }
 ;
 
int tmp9 = OPTION_CAST(int)( ((int)( res ) )); return opt_ok(&tmp9, sizeof(int)) ;
 
 }
 Option_int net__Socket_bind(net__Socket s, int port) {
  
 struct /*c struct init*/ 

sockaddr_in addr ;
 
 addr .sin_family  =  s .family ;
 
 addr .sin_port  =  htons ( port ) ;
 
 addr .sin_addr .s_addr  =  htonl ( INADDR_ANY ) ;
 
int size= 16 ;
 
int res= ((int)( bind ( s .sockfd ,  & /*v*/  addr ,  size ) ) ) ;
 
 if ( res < 0 ) {
 
return  v_error ( tos2((byte*)"socket: bind failed") ) ;
 
 }
 ;
 
int tmp13 = OPTION_CAST(int)( res); return opt_ok(&tmp13, sizeof(int)) ;
 
 }
 Option_int net__Socket_listen(net__Socket s) {
 
int backlog= 128 ;
 
int res= ((int)( listen ( s .sockfd ,  backlog ) ) ) ;
 
 if ( res < 0 ) {
 
return  v_error ( tos2((byte*)"socket: listen failed") ) ;
 
 }
 ;
 
 #ifdef VDEBUG
 
printf( "listen res = %d\n", res ) ;
 
 #endif
 ;
 
int tmp16 = OPTION_CAST(int)( res); return opt_ok(&tmp16, sizeof(int)) ;
 
 }
 Option_int net__Socket_listen_backlog(net__Socket s, int backlog) {
 
int n= 0 ;
 
 if ( backlog > 0 ) {
 
 n  =  backlog ;
 
 }
 ;
 
void* res= listen ( s .sockfd ,  n ) ;
 
 if ( res < 0 ) {
 
return  v_error ( tos2((byte*)"socket: listen_backlog failed") ) ;
 
 }
 ;
 
int tmp19 = OPTION_CAST(int)( ((int)( res ) )); return opt_ok(&tmp19, sizeof(int)) ;
 
 }
 Option_net__Socket net__listen(int port) {
 
 #ifdef VDEBUG
 
printf( "net.listen(%d)\n", port ) ;
 
 #endif
 ;
 
Option_net__Socket tmp20 =  net__socket ( AF_INET , SOCK_STREAM , 0 ) ;
 if (!tmp20 .ok) {
 string err = tmp20 . error;
 
return  v_error ( err ) ;
 
 }
 net__Socket s = *(net__Socket*) tmp20 . data;
 ;
 
Option_int tmp21 =  net__Socket_bind( s , port ) ;
 if (!tmp21 .ok) {
 string err = tmp21 . error;
 
return  v_error ( err ) ;
 
 }
 int bind_res = *(int*) tmp21 . data;
 ;
 
Option_int tmp22 =  net__Socket_listen( s ) ;
 if (!tmp22 .ok) {
 string err = tmp22 . error;
 
return  v_error ( err ) ;
 
 }
 int listen_res = *(int*) tmp22 . data;
 ;
 
net__Socket tmp23 = OPTION_CAST(net__Socket)( s); return opt_ok(&tmp23, sizeof(net__Socket)) ;
 
 }
 Option_net__Socket net__Socket_accept(net__Socket s) {
 
 #ifdef VDEBUG
 
 println ( tos2((byte*)"accept()") ) ;
 
 #endif
 ;
  
 struct /*c struct init*/ 

sockaddr_storage addr ;
 
int size= 128 ;
 
void* sockfd= accept ( s .sockfd ,  & /*v*/  addr ,  & /*v*/  size ) ;
 
 if ( sockfd < 0 ) {
 
return  v_error ( tos2((byte*)"socket: accept failed") ) ;
 
 }
 ;
 
net__Socket c= (net__Socket) { .sockfd =  sockfd , .family =  s .family , ._type =  s ._type , .proto =  s .proto } ;
 
net__Socket tmp28 = OPTION_CAST(net__Socket)( c); return opt_ok(&tmp28, sizeof(net__Socket)) ;
 
 }
 Option_int net__Socket_connect(net__Socket s, string address, int port) {
  
 struct /*c struct init*/ 

addrinfo hints ;
 
 hints .ai_family  =  AF_UNSPEC ;
 
 hints .ai_socktype  =  SOCK_STREAM ;
 
 hints .ai_flags  =  AI_PASSIVE ;
 
 hints .ai_addrlen  =  0 ;
 
 hints .ai_canonname  =  NULL ;
 
 hints .ai_addr  =  NULL ;
  
 struct /*c struct init*/ 

addrinfo* info= 0 ;
 
string sport= _STR("%d", port) ;
 
void* info_res= getaddrinfo ((char*) address .str , (char*) sport .str ,  & /*v*/  hints ,  & /*v*/  info ) ;
 
 if ( info_res != 0 ) {
 
return  v_error ( tos2((byte*)"socket: connect failed") ) ;
 
 }
 ;
 
int res= ((int)( connect ( s .sockfd ,  info ->ai_addr ,  info ->ai_addrlen ) ) ) ;
 
 if ( res < 0 ) {
 
return  v_error ( tos2((byte*)"socket: connect failed") ) ;
 
 }
 ;
 
int tmp34 = OPTION_CAST(int)( ((int)( res ) )); return opt_ok(&tmp34, sizeof(int)) ;
 
 }
 Option_net__Socket net__dial(string address, int port) {
 
Option_net__Socket tmp35 =  net__socket ( AF_INET , SOCK_STREAM , 0 ) ;
 if (!tmp35 .ok) {
 string err = tmp35 . error;
 
return  v_error ( err ) ;
 
 }
 net__Socket s = *(net__Socket*) tmp35 . data;
 ;
 
Option_int tmp36 =  net__Socket_connect( s , address , port ) ;
 if (!tmp36 .ok) {
 string err = tmp36 . error;
 
return  v_error ( err ) ;
 
 }
 int res = *(int*) tmp36 . data;
 ;
 
net__Socket tmp37 = OPTION_CAST(net__Socket)( s); return opt_ok(&tmp37, sizeof(net__Socket)) ;
 
 }
 int net__Socket_send(net__Socket s, byte* buf, int len) {
 
void* res= send ( s .sockfd , (char*) buf ,  len ,  0 ) ;
 
return  res ;
 
 }
 byte* net__Socket_recv(net__Socket s, int bufsize) {
 
byte* buf= v_malloc ( bufsize ) ;
 
void* res= recv ( s .sockfd , (char*) buf ,  bufsize ,  0 ) ;
 
return  buf ;
 
 }
 int net__Socket_cread(net__Socket s, byte* buffer, int buffersize) {
 
return  ((int)( read ( s .sockfd , (char*) buffer ,  buffersize ) ) ) ;
 
 }
 int net__Socket_crecv(net__Socket s, byte* buffer, int buffersize) {
 
return  ((int)( recv ( s .sockfd , (char*) buffer ,  buffersize ,  0 ) ) ) ;
 
 }
 Option_int net__Socket_close(net__Socket s) {
 
int shutdown_res= 0 ;
 
 #ifdef _WIN32
 
 shutdown_res  =  shutdown ( s .sockfd ,  SD_BOTH ) ;
 
 #else
 
 shutdown_res  =  shutdown ( s .sockfd ,  SHUT_RDWR ) ;
 
 #endif
 ;
 
int res= 0 ;
 
 #ifdef _WIN32
 
 res  =  closesocket ( s .sockfd ) ;
 
 #else
 
 res  =  close ( s .sockfd ) ;
 
 #endif
 ;
 
 if ( res < 0 ) {
 
return  v_error ( tos2((byte*)"socket: close failed") ) ;
 
 }
 ;
 
int tmp43 = OPTION_CAST(int)( 0); return opt_ok(&tmp43, sizeof(int)) ;
 
 }
 void net__Socket_write(net__Socket s, string str) {
 
string line= _STR("%.*s\r\n", str.len, str.str) ;
 
 send ( s .sockfd , (char*) line .str ,  line .len ,  0 ) ;
 
 }
 string net__Socket_read_line(net__Socket s) {
 
string res= tos2((byte*)"") ;
 
 while (1) { 
 
 #ifdef VDEBUG
 
 println ( tos2((byte*)".") ) ;
 
 #endif
 ;
 
byte* buf= v_malloc ( net__MAX_READ ) ;
 
int n= ((int)( recv ( s .sockfd , (char*) buf ,  net__MAX_READ - 1 ,  0 ) ) ) ;
 
 #ifdef VDEBUG
 
printf( "numbytes=%d\n", n ) ;
 
 #endif
 ;
 
 if ( n == - 1 ) {
 
 #ifdef VDEBUG
 
 println ( tos2((byte*)"recv failed") ) ;
 
 #endif
 ;
 
return  tos2((byte*)"") ;
 
 }
 ;
 
 if ( n == 0 ) {
 
 break
 ;
 
 }
 ;
 
 buf [/*ptr*/ n ]/*rbyte 1*/  =  '\0' ;
 
string line= (tos2((byte *) buf ) ) ;
 
 res = string_add(res,  line ) ;
 
 if ( string_ends_with( line , tos2((byte*)"\n") )  ||  n < net__MAX_READ - 1 ) {
 
 break
 ;
 
 }
 ;
 
 if ( string_ends_with( line , tos2((byte*)"\r\n") ) ) {
 
 break
 ;
 
 }
 ;
 
 }
 ;
 
return  res ;
 
 }
 int net__Socket_get_port(net__Socket s) {
  
 struct /*c struct init*/ 

sockaddr_in addr ;
 
int size= 16 ;
 
void* sockname_res= getsockname ( s .sockfd ,  & /*v*/  addr ,  & /*v*/  size ) ;
 
return  ((int)( ntohs ( addr .sin_port ) ) ) ;
 
 }
 array_string os__parse_windows_cmd_line(byte* cmd) {
 
string s= (tos2((byte *) cmd ) ) ;
 
return  string_split( s , tos2((byte*)" ") ) ;
 
 }
 Option_string os__read_file(string path) {
 
string mode= tos2((byte*)"rb") ;
 
FILE* fp= os__vfopen ( path , mode ) ;
 
 if ( isnil ( fp ) ) {
 
return  v_error ( _STR("failed to open file \"%.*s\"", path.len, path.str) ) ;
 
 }
 ;
 
 fseek ( fp ,  0 ,  SEEK_END ) ;
 
int fsize= ftell ( fp ) ;
 
 rewind ( fp ) ;
 
byte* str= v_malloc ( fsize + 1 ) ;
 
 fread ((char*) str ,  fsize ,  1 ,  fp ) ;
 
 fclose ( fp ) ;
 
 str [/*ptr*/ fsize ]/*rbyte 1*/  =  0 ;
 
string tmp6 = OPTION_CAST(string)( (tos((byte *) str ,  fsize ) )); return opt_ok(&tmp6, sizeof(string)) ;
 
 }
 int os__file_size(string path) {
  
 struct /*c struct init*/ 

stat s ;
 
 #ifdef _WIN32
 
 _wstat ( string_to_wide( path ) ,  & /*v*/  s ) ;
 
 #else
 
 stat ( ((char*)( path .str ) ) ,  & /*v*/  s ) ;
 
 #endif
 ;
 
return  s .st_size ;
 
 }
 void os__mv(string old, string new) {
 
 #ifdef _WIN32
 
 _wrename ( string_to_wide( old ) ,  string_to_wide( new ) ) ;
 
 #else
 
 rename ( ((char*)( old .str ) ) ,  ((char*)( new .str ) ) ) ;
 
 #endif
 ;
 
 }
 FILE* os__vfopen(string path, string mode) {
 
 #ifdef _WIN32
 
return  _wfopen ( string_to_wide( path ) ,  string_to_wide( mode ) ) ;
 
 #else
 
return  fopen ( ((char*)( path .str ) ) ,  ((char*)( mode .str ) ) ) ;
 
 #endif
 ;
 
 }
 array_string os__read_lines(string path) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
int buf_len= 1024 ;
 
byte* buf= v_malloc ( buf_len ) ;
 
string mode= tos2((byte*)"rb") ;
 
FILE* fp= os__vfopen ( path , mode ) ;
 
 if ( isnil ( fp ) ) {
 
return  res ;
 
 }
 ;
 
int buf_index= 0 ;
 
 while ( fgets ((char*) buf + buf_index ,  buf_len - buf_index ,  fp ) != 0 ) {
 
 
int len= vstrlen ( buf ) ;
 
 if ( len == buf_len - 1  &&  buf [/*ptr*/ len - 1 ]/*rbyte 1*/ != 10 ) {
 
 buf_len  *=  2 ;
 
 buf  =  realloc ((char*) buf ,  buf_len ) ;
 
 if ( isnil ( buf ) ) {
 
 v_panic ( tos2((byte*)"Could not reallocate the read buffer") ) ;
 
 }
 ;
 
 buf_index  =  len ;
 
 continue
 ;
 
 }
 ;
 
 if ( buf [/*ptr*/ len - 1 ]/*rbyte 1*/ == 10  ||  buf [/*ptr*/ len - 1 ]/*rbyte 1*/ == 13 ) {
 
 buf [/*ptr*/ len - 1 ]/*rbyte 1*/  =  '\0' ;
 
 }
 ;
 
 if ( len > 1  &&  buf [/*ptr*/ len - 2 ]/*rbyte 1*/ == 13 ) {
 
 buf [/*ptr*/ len - 2 ]/*rbyte 1*/  =  '\0' ;
 
 }
 ;
 
_PUSH(& res , ( tos_clone ( buf ) ), tmp15, string) ;
 
 buf_index  =  0 ;
 
 }
 ;
 
 fclose ( fp ) ;
 
return  res ;
 
 }
 array_ustring os__read_ulines(string path) {
 
array_string lines= os__read_lines ( path ) ;
 
array_ustring ulines=new_array_from_c_array(0, 0, sizeof(ustring), (ustring[]) {   0 }) ;
 
 array_string tmp18 =  lines;
 for (int tmp19 = 0; tmp19 < tmp18.len; tmp19++) {
 string myline = ((string *) tmp18 . data)[tmp19];
 
 
_PUSH(& ulines , ( string_ustring( myline ) ), tmp20, ustring) ;
 
 }
 ;
 
return  ulines ;
 
 }
 Option_os__File os__open(string path) {
 
os__File file= (os__File) { .cfile =  0 } ;
 
 #ifdef _WIN32
 
u16* wpath= string_to_wide( path ) ;
 
string mode= tos2((byte*)"rb") ;
 
 file  =  (os__File) { .cfile =  _wfopen ( wpath ,  string_to_wide( mode ) ) } ;
 
 #else
 
byte* cpath= path .str ;
 
 file  =  (os__File) { .cfile =  fopen ( ((char*)( cpath ) ) ,  "rb" ) } ;
 
 #endif
 ;
 
 if ( isnil ( file .cfile ) ) {
 
return  v_error ( _STR("failed to open file \"%.*s\"", path.len, path.str) ) ;
 
 }
 ;
 
os__File tmp25 = OPTION_CAST(os__File)( file); return opt_ok(&tmp25, sizeof(os__File)) ;
 
 }
 Option_os__File os__create(string path) {
 
os__File file= (os__File) { .cfile =  0 } ;
 
 #ifdef _WIN32
 
u16* wpath= string_to_wide( string_replace( path , tos2((byte*)"/") , tos2((byte*)"\\") ) ) ;
 
string mode= tos2((byte*)"wb") ;
 
 file  =  (os__File) { .cfile =  _wfopen ( wpath ,  string_to_wide( mode ) ) } ;
 
 #else
 
byte* cpath= path .str ;
 
 file  =  (os__File) { .cfile =  fopen ( ((char*)( cpath ) ) ,  "wb" ) } ;
 
 #endif
 ;
 
 if ( isnil ( file .cfile ) ) {
 
return  v_error ( _STR("failed to create file \"%.*s\"", path.len, path.str) ) ;
 
 }
 ;
 
os__File tmp30 = OPTION_CAST(os__File)( file); return opt_ok(&tmp30, sizeof(os__File)) ;
 
 }
 Option_os__File os__open_append(string path) {
 
os__File file= (os__File) { .cfile =  0 } ;
 
 #ifdef _WIN32
 
u16* wpath= string_to_wide( string_replace( path , tos2((byte*)"/") , tos2((byte*)"\\") ) ) ;
 
string mode= tos2((byte*)"ab") ;
 
 file  =  (os__File) { .cfile =  _wfopen ( wpath ,  string_to_wide( mode ) ) } ;
 
 #else
 
byte* cpath= path .str ;
 
 file  =  (os__File) { .cfile =  fopen ( ((char*)( cpath ) ) ,  "ab" ) } ;
 
 #endif
 ;
 
 if ( isnil ( file .cfile ) ) {
 
return  v_error ( _STR("failed to create(append) file \"%.*s\"", path.len, path.str) ) ;
 
 }
 ;
 
os__File tmp35 = OPTION_CAST(os__File)( file); return opt_ok(&tmp35, sizeof(os__File)) ;
 
 }
 void os__File_write(os__File f, string s) {
 
 fputs ((char*) s .str ,  f .cfile ) ;
 
 }
 void os__File_write_bytes(os__File f, void* data, int size) {
 
 fwrite ( data ,  1 ,  size ,  f .cfile ) ;
 
 }
 void os__File_write_bytes_at(os__File f, void* data, int size, int pos) {
 
 fseek ( f .cfile ,  pos ,  SEEK_SET ) ;
 
 fwrite ( data ,  1 ,  size ,  f .cfile ) ;
 
 fseek ( f .cfile ,  0 ,  SEEK_END ) ;
 
 }
 void os__File_writeln(os__File f, string s) {
 
 fputs ((char*) s .str ,  f .cfile ) ;
 
 fputs ( "\n" ,  f .cfile ) ;
 
 }
 void os__File_flush(os__File f) {
 
 fflush ( f .cfile ) ;
 
 }
 void os__File_close(os__File f) {
 
 fclose ( f .cfile ) ;
 
 }
 FILE* os__popen(string path) {
 
 #ifdef _WIN32
 
string mode= tos2((byte*)"rb") ;
 
u16* wpath= string_to_wide( path ) ;
 
return  _wpopen ( wpath ,  string_to_wide( mode ) ) ;
 
 #else
 
byte* cpath= path .str ;
 
return  popen ((char*) cpath ,  "r" ) ;
 
 #endif
 ;
 
 }
 int os__pclose(FILE* f) {
 
 #ifdef _WIN32
 
return  _pclose ( f ) ;
 
 #else
 
return  pclose ( f ) / 256 ;
 
 #endif
 ;
 
 }
 Option_os__Result os__exec(string cmd) {
 
string pcmd= _STR("%.*s 2>&1", cmd.len, cmd.str) ;
 
FILE* f= os__popen ( pcmd ) ;
 
 if ( isnil ( f ) ) {
 
return  v_error ( _STR("exec(\"%.*s\") failed", cmd.len, cmd.str) ) ;
 
 }
 ;
 
byte buf  [1000 ] ;
 
string res= tos2((byte*)"") ;
 
 while ( fgets ( ((char*)( buf ) ) ,  1000 ,  f ) != 0 ) {
 
 
 res = string_add(res,  tos ( buf , vstrlen ( buf ) ) ) ;
 
 }
 ;
 
 res  =  string_trim_space( res ) ;
 
int exit_code= os__pclose ( f ) ;
 
os__Result tmp44 = OPTION_CAST(os__Result)( (os__Result) { .output =  res , .exit_code =  exit_code }); return opt_ok(&tmp44, sizeof(os__Result)) ;
 
 }
 int os__system(string cmd) {
 
int ret= ((int)( 0 ) ) ;
 
 #ifdef _WIN32
 
 ret  =  _wsystem ( string_to_wide( cmd ) ) ;
 
 #else
 
 ret  =  system ((char*) cmd .str ) ;
 
 #endif
 ;
 
 if ( ret == - 1 ) {
 
 os__print_c_errno ( ) ;
 
 }
 ;
 
return  ret ;
 
 }
 string os__getenv(string key) {
 
 #ifdef _WIN32
 
void* s= _wgetenv ( string_to_wide( key ) ) ;
 
 if ( isnil ( s ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_from_wide ( s ) ;
 
 #else
 
byte* s= ((byte*)( getenv ((char*) key .str ) ) ) ;
 
 if ( isnil ( s ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  (tos2((byte *) s ) ) ;
 
 #endif
 ;
 
 }
 int os__setenv(string name, string value, bool overwrite) {
 
 #ifdef _WIN32
 
string format= _STR("%.*s=%.*s", name.len, name.str, value.len, value.str) ;
 
 if ( overwrite ) {
 
return  _putenv ((char*) format .str ) ;
 
 }
 ;
 
return  - 1 ;
 
 #else
 
return  setenv ((char*) name .str , (char*) value .str ,  overwrite ) ;
 
 #endif
 ;
 
 }
 int os__unsetenv(string name) {
 
 #ifdef _WIN32
 
string format= _STR("%.*s=", name.len, name.str) ;
 
return  _putenv ((char*) format .str ) ;
 
 #else
 
return  unsetenv ((char*) name .str ) ;
 
 #endif
 ;
 
 }
 bool os__file_exists(string _path) {
 
 #ifdef _WIN32
 
string path= string_replace( _path , tos2((byte*)"/") , tos2((byte*)"\\") ) ;
 
return  _waccess ( string_to_wide( path ) ,  0 ) != - 1 ;
 
 #else
 
return  access ((char*) _path .str ,  0 ) != - 1 ;
 
 #endif
 ;
 
 }
 void os__rm(string path) {
 
 #ifdef _WIN32
 
 _wremove ( string_to_wide( path ) ) ;
 
 #else
 
 remove ((char*) path .str ) ;
 
 #endif
 ;
 
 }
 void os__rmdir(string path) {
 
 #ifndef _WIN32
 
 rmdir ((char*) path .str ) ;
 
 #else
 
 RemoveDirectory ( string_to_wide( path ) ) ;
 
 #endif
 ;
 
 }
 void os__print_c_errno() {
 
 }
 string os__ext(string path) {
 
int pos= string_last_index( path , tos2((byte*)".") ) ;
 
 if ( pos == - 1 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_right( path , pos ) ;
 
 }
 string os__dir(string path) {
 
 if (string_eq( path , tos2((byte*)".") ) ) {
 
return  os__getwd ( ) ;
 
 }
 ;
 
int pos= string_last_index( path , os__PathSeparator ) ;
 
 if ( pos == - 1 ) {
 
return  tos2((byte*)".") ;
 
 }
 ;
 
return  string_left( path , pos ) ;
 
 }
 string os__path_sans_ext(string path) {
 
int pos= string_last_index( path , tos2((byte*)".") ) ;
 
 if ( pos == - 1 ) {
 
return  path ;
 
 }
 ;
 
return  string_left( path , pos ) ;
 
 }
 string os__basedir(string path) {
 
int pos= string_last_index( path , os__PathSeparator ) ;
 
 if ( pos == - 1 ) {
 
return  path ;
 
 }
 ;
 
return  string_left( path , pos + 1 ) ;
 
 }
 string os__filename(string path) {
 
return  string_all_after( path , os__PathSeparator ) ;
 
 }
 string os__get_line() {
 
string str= os__get_raw_line ( ) ;
 
 #ifdef _WIN32
 
return  string_trim_right( str , tos2((byte*)"\r\n") ) ;
 
 #else
 
return  string_trim_right( str , tos2((byte*)"\n") ) ;
 
 #endif
 ;
 
 }
 string os__get_raw_line() {
 
 #ifdef _WIN32
 
int maxlinechars= 256 ;
 
byte* buf= ((byte*)( v_malloc ( maxlinechars * 2 ) ) ) ;
 
int res= ((int)( fgetws ((char*) buf ,  maxlinechars ,  stdin ) ) ) ;
 
int len= ((int)( wcslen ( ((u16*)( buf ) ) ) ) ) ;
 
 if ( 0 != res ) {
 
return  string_from_wide2 ( ((u16*)( buf ) ) , len ) ;
 
 }
 ;
 
return  tos2((byte*)"") ;
 
 #else
 
size_t max= ((size_t)( 256 ) ) ;
 
char* buf= ((char*)( v_malloc ( ((int)( max ) ) ) ) ) ;
 
int nr_chars= getline ( & /*v*/  buf ,  & /*v*/  max ,  stdin ) ;
 
 if ( nr_chars == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  (tos((byte *) ((byteptr)( buf ) ) ,  nr_chars ) ) ;
 
 #endif
 ;
 
 }
 array_string os__get_lines() {
 
string line= tos2((byte*)"") ;
 
array_string inputstr=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 while (1) { 
 
 line  =  os__get_line ( ) ;
 
 if ( ( line .len <= 0 ) ) {
 
 break
 ;
 
 }
 ;
 
 line  =  string_trim_space( line ) ;
 
_PUSH(& inputstr , ( line ), tmp65, string) ;
 
 }
 ;
 
return  inputstr ;
 
 }
 string os__get_lines_joined() {
 
string line= tos2((byte*)"") ;
 
string inputstr= tos2((byte*)"") ;
 
 while (1) { 
 
 line  =  os__get_line ( ) ;
 
 if ( ( line .len <= 0 ) ) {
 
 break
 ;
 
 }
 ;
 
 line  =  string_trim_space( line ) ;
 
 inputstr = string_add(inputstr,  line ) ;
 
 }
 ;
 
return  inputstr ;
 
 }
 string os__user_os() {
 
 #ifdef __linux__
 
return  tos2((byte*)"linux") ;
 
 #endif
 ;
 
 #ifdef __APPLE__
 
return  tos2((byte*)"mac") ;
 
 #endif
 ;
 
 #ifdef _WIN32
 
return  tos2((byte*)"windows") ;
 
 #endif
 ;
 
 #ifdef __FreeBSD__
 
return  tos2((byte*)"freebsd") ;
 
 #endif
 ;
 
 #ifdef __OpenBSD__
 
return  tos2((byte*)"openbsd") ;
 
 #endif
 ;
 
 #ifdef __NetBSD__
 
return  tos2((byte*)"netbsd") ;
 
 #endif
 ;
 
 #ifdef __DragonFly__
 
return  tos2((byte*)"dragonfly") ;
 
 #endif
 ;
 
 #ifdef _MSC_VER
 
return  tos2((byte*)"windows") ;
 
 #endif
 ;
 
 #ifdef __BIONIC__
 
return  tos2((byte*)"android") ;
 
 #endif
 ;
 
return  tos2((byte*)"unknown") ;
 
 }
 string os__home_dir() {
 
string home= os__getenv ( tos2((byte*)"HOME") ) ;
 
 #ifdef _WIN32
 
 home  =  os__getenv ( tos2((byte*)"HOMEDRIVE") ) ;
 
 if ( home .len == 0 ) {
 
 home  =  os__getenv ( tos2((byte*)"SYSTEMDRIVE") ) ;
 
 }
 ;
 
string homepath= os__getenv ( tos2((byte*)"HOMEPATH") ) ;
 
 if ( homepath .len == 0 ) {
 
 homepath  = string_add( tos2((byte*)"\\Users\\") , os__getenv ( tos2((byte*)"USERNAME") ) ) ;
 
 }
 ;
 
 home = string_add(home,  homepath ) ;
 
 #endif
 ;
 
 home = string_add(home,  os__PathSeparator ) ;
 
return  home ;
 
 }
 void os__write_file(string path, string text) {
 
Option_os__File tmp70 =  os__create ( path ) ;
 if (!tmp70 .ok) {
 string err = tmp70 . error;
 
 return ;
 
 }
 os__File f = *(os__File*) tmp70 . data;
 ;
 
 os__File_write( f , text ) ;
 
 os__File_close( f ) ;
 
 }
 void os__clear() {
 
 printf ( "\x1b[2J" ) ;
 
 printf ( "\x1b[H" ) ;
 
 }
 void os__on_segfault(void* f) {
 
 #ifdef _WIN32
 
 return ;
 
 #endif
 ;
 
 #ifdef __APPLE__
  
 struct /*c struct init*/ 

sigaction sa ;
 
 memset ( & /*v*/  sa ,  0 ,  sizeof( sigaction) ) ;
 
 sigemptyset ( & /*v*/  sa .sa_mask ) ;
 
 sa .sa_sigaction  =  f ;
 
 sa .sa_flags  =  SA_SIGINFO ;
 
 sigaction ( SIGSEGV ,  & /*v*/  sa ,  0 ) ;
 
 #endif
 ;
 
 }
 string os__executable() {
 
 #ifdef __linux__
 
byte* result= v_malloc ( os__MAX_PATH ) ;
 
int count= ((int)( readlink ( "/proc/self/exe" , (char*) result ,  os__MAX_PATH ) ) ) ;
 
 if ( count < 0 ) {
 
 v_panic ( tos2((byte*)"error reading /proc/self/exe to get exe path") ) ;
 
 }
 ;
 
return  (tos((byte *) result ,  count ) ) ;
 
 #endif
 ;
 
 #ifdef _WIN32
 
int max= 512 ;
 
u16* result= ((u16*)( v_malloc ( max * 2 ) ) ) ;
 
int len= ((int)( GetModuleFileName ( 0 ,  result ,  max ) ) ) ;
 
return  string_from_wide2 ( result , len ) ;
 
 #endif
 ;
 
 #ifdef __APPLE__
 
byte* result= v_malloc ( os__MAX_PATH ) ;
 
int pid= getpid ( ) ;
 
int ret= proc_pidpath ( pid , (char*) result ,  os__MAX_PATH ) ;
 
 if ( ret <= 0 ) {
 
 println ( tos2((byte*)"os.executable() failed") ) ;
 
return  tos2((byte*)".") ;
 
 }
 ;
 
return  (tos2((byte *) result ) ) ;
 
 #endif
 ;
 
 #ifdef __FreeBSD__
 
byte* result= v_malloc ( os__MAX_PATH ) ;
 
array_int mib=new_array_from_c_array(4, 4, sizeof(int), (int[]) {  1 ,  14 ,  12 ,  - 1  }) ;
 
int size= os__MAX_PATH ;
 
 sysctl ( mib .data ,  4 , (char*) result ,  & /*v*/  size ,  0 ,  0 ) ;
 
return  (tos2((byte *) result ) ) ;
 
 #endif
 ;
 
 #ifdef __OpenBSD__
 
return  ( *(string*) array__get( os__args , 0) ) ;
 
 #endif
 ;
 
 #ifdef __NetBSD__
 
byte* result= v_malloc ( os__MAX_PATH ) ;
 
int count= ((int)( readlink ( "/proc/curproc/exe" , (char*) result ,  os__MAX_PATH ) ) ) ;
 
 if ( count < 0 ) {
 
 v_panic ( tos2((byte*)"error reading /proc/curproc/exe to get exe path") ) ;
 
 }
 ;
 
return  (tos((byte *) result ,  count ) ) ;
 
 #endif
 ;
 
 #ifdef __DragonFly__
 
byte* result= v_malloc ( os__MAX_PATH ) ;
 
int count= ((int)( readlink ( "/proc/curproc/file" , (char*) result ,  os__MAX_PATH ) ) ) ;
 
 if ( count < 0 ) {
 
 v_panic ( tos2((byte*)"error reading /proc/curproc/file to get exe path") ) ;
 
 }
 ;
 
return  (tos((byte *) result ,  count ) ) ;
 
 #endif
 ;
 
return  tos2((byte*)".") ;
 
 }
 bool os__is_dir(string path) {
 
 #ifdef _WIN32
 
return  os__dir_exists ( path ) ;
 
 #else
  
 struct /*c struct init*/ 

stat statbuf ;
 
byte* cstr= path .str ;
 
 if ( stat ((char*) cstr ,  & /*v*/  statbuf ) != 0 ) {
 
return  0 ;
 
 }
 ;
 
return  ( statbuf .st_mode & os__S_IFMT ) == os__S_IFDIR ;
 
 #endif
 ;
 
 }
 void os__chdir(string path) {
 
 #ifdef _WIN32
 
 _wchdir ( string_to_wide( path ) ) ;
 
 #else
 
 chdir ((char*) path .str ) ;
 
 #endif
 ;
 
 }
 string os__getwd() {
 
 #ifdef _WIN32
 
int max= 512 ;
 
u16* buf= ((u16*)( v_malloc ( max * 2 ) ) ) ;
 
 if ( _wgetcwd ( buf ,  max ) == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_from_wide ( buf ) ;
 
 #else
 
byte* buf= v_malloc ( 512 ) ;
 
 if ( getcwd ((char*) buf ,  512 ) == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  (tos2((byte *) buf ) ) ;
 
 #endif
 ;
 
 }
 string os__realpath(string fpath) {
 
byte* fullpath= v_malloc ( os__MAX_PATH ) ;
 
int res= 0 ;
 
 #ifdef _WIN32
 
 res  =  ((int)( _fullpath ((char*) fullpath , (char*) fpath .str ,  os__MAX_PATH ) ) ) ;
 
 #else
 
 res  =  ((int)( realpath ((char*) fpath .str , (char*) fullpath ) ) ) ;
 
 #endif
 ;
 
 if ( res != 0 ) {
 
return  (tos((byte *) fullpath ,  vstrlen ( fullpath ) ) ) ;
 
 }
 ;
 
return  fpath ;
 
 }
 array_string os__walk_ext(string path, string ext) {
 
 if ( ! os__is_dir ( path ) ) {
 
return new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 }
 ;
 
array_string files= os__ls ( path ) ;
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 array_string tmp98 =  files;
 for (int i = 0; i < tmp98.len; i++) {
 string file = ((string *) tmp98 . data)[i];
 
 
 if ( string_starts_with( file , tos2((byte*)".") ) ) {
 
 continue
 ;
 
 }
 ;
 
string p=string_add(string_add( path , os__PathSeparator ) , file ) ;
 
 if ( os__is_dir ( p ) ) {
 
_PUSH_MANY(& res , ( os__walk_ext ( p , ext ) ), tmp100, array_string) ;
 
 }
  else  if ( string_ends_with( file , ext ) ) {
 
_PUSH(& res , ( p ), tmp101, string) ;
 
 }
 ;
 
 }
 ;
 
return  res ;
 
 }
 void os__signal(int signum, void* handler) {
 
 signal ( signum ,  handler ) ;
 
 }
 int os__fork() {
 
 #ifndef _WIN32
 
int pid= fork ( ) ;
 
return  pid ;
 
 #endif
 ;
 
 v_panic ( tos2((byte*)"os.fork not supported in windows") ) ;
 
 }
 int os__wait() {
 
 #ifndef _WIN32
 
int pid= wait ( 0 ) ;
 
return  pid ;
 
 #endif
 ;
 
 v_panic ( tos2((byte*)"os.wait not supported in windows") ) ;
 
 }
 int os__file_last_mod_unix(string path) {
  
 struct /*c struct init*/ 

stat attr ;
 
 stat ((char*) path .str ,  & /*v*/  attr ) ;
 
return  attr .st_mtime ;
 
 }
 void os__log(string s) {
 
 }
 void os__flush_stdout() {
 
 fflush ( stdout ) ;
 
 }
 void os__print_backtrace() {
 
 }
 array_string os__init_os_args(int argc, byteptr* argv) {
 
array_string args=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 for (
int i= 0  ;  i < argc  ;  i ++ ) { 
 
 
_PUSH(& args , ( (tos2((byte *) argv [/*ptr*/ i ]/*rbyteptr 0*/ ) ) ), tmp3, string) ;
 
 }
 ;
 
return  args ;
 
 }
 string os__get_error_msg(int code) {
 
void* _ptr_text= strerror ( code ) ;
 
 if ( _ptr_text == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  tos ( _ptr_text , vstrlen ( _ptr_text ) ) ;
 
 }
 array_string os__ls(string path) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
void* dir= opendir ((char*) path .str ) ;
 
 if ( isnil ( dir ) ) {
 
printf( "ls() couldnt open dir \"%.*s\"\n", path.len, path.str ) ;
 
 os__print_c_errno ( ) ;
 
return  res ;
 
 }
 ;
  
 struct /*c struct init*/ 

dirent* ent= 0 ;
 
 while (1) { 
 
 ent  =  readdir ( dir ) ;
 
 if ( isnil ( ent ) ) {
 
 break
 ;
 
 }
 ;
 
string name= tos_clone ( ((byteptr)( ent ->d_name ) ) ) ;
 
 if (string_ne( name , tos2((byte*)".") )  && string_ne( name , tos2((byte*)"..") )  && string_ne( name , tos2((byte*)"") ) ) {
 
_PUSH(& res , ( name ), tmp9, string) ;
 
 }
 ;
 
 }
 ;
 
 closedir ( dir ) ;
 
return  res ;
 
 }
 bool os__dir_exists(string path) {
 
void* dir= opendir ((char*) path .str ) ;
 
bool res= ! isnil ( dir ) ;
 
 if ( res ) {
 
 closedir ( dir ) ;
 
 }
 ;
 
return  res ;
 
 }
 void os__mkdir(string path) {
 
 mkdir ((char*) path .str ,  511 ) ;
 
 }
 string net_dot_urllib__error_msg(string message, string val) {
 
string msg= _STR("net.urllib: %.*s", message.len, message.str) ;
 
 if (string_ne( val , tos2((byte*)"") ) ) {
 
 msg  =  _STR("%.*s (%.*s)", msg.len, msg.str, val.len, val.str) ;
 
 }
 ;
 
return  msg ;
 
 }
 bool net_dot_urllib__should_escape(byte c, net_dot_urllib__EncodingMode mode) {
 
 if ( ( 'a' <= c  &&  c <= 'z' )  ||  ( 'A' <= c  &&  c <= 'Z' )  ||  ( '0' <= c  &&  c <= '9' ) ) {
 
return  0 ;
 
 }
 ;
 
 if ( mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_host  ||  mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_zone ) {
 
 if (  c ==  '!'  ||   c ==  '$'  ||   c ==  '&'  ||   c ==  '\\'  ||   c ==  '('  ||   c ==  ')'  ||   c ==  '*'  ||   c ==  '+'  ||   c ==  ','  ||   c ==  ';'  ||   c ==  '='  ||   c ==  ':'  ||   c ==  '['  ||   c ==  ']'  ||   c ==  '<'  ||   c ==  '>'  ||   c ==  '"' ) { /* case */
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
 if (  c ==  '-'  ||   c ==  '_'  ||   c ==  '.'  ||   c ==  '~' ) { /* case */
 
return  0 ;
 
 }
 else  if (  c ==  '$'  ||   c ==  '&'  ||   c ==  '+'  ||   c ==  ','  ||   c ==  '/'  ||   c ==  ':'  ||   c ==  ';'  ||   c ==  '='  ||   c ==  '?'  ||   c ==  '@' ) { /* case */
 
 if (  mode ==  net_dot_urllib__net_dot_urllib__EncodingMode_encode_path ) { /* case */
 
return  c == '?' ;
 
 }
 else  if (  mode ==  net_dot_urllib__net_dot_urllib__EncodingMode_encode_path_segment ) { /* case */
 
return  c == '/'  ||  c == ';'  ||  c == ','  ||  c == '?' ;
 
 }
 else  if (  mode ==  net_dot_urllib__net_dot_urllib__EncodingMode_encode_user_password ) { /* case */
 
return  c == '@'  ||  c == '/'  ||  c == '?'  ||  c == ':' ;
 
 }
 else  if (  mode ==  net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component ) { /* case */
 
return  1 ;
 
 }
 else  if (  mode ==  net_dot_urllib__net_dot_urllib__EncodingMode_encode_fragment ) { /* case */
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
 if ( mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_fragment ) {
 
 if (  c ==  '!'  ||   c ==  '('  ||   c ==  ')'  ||   c ==  '*' ) { /* case */
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
return  1 ;
 
 }
 Option_string net_dot_urllib__query_unescape(string s) {
 
Option_string tmp2 = OPTION_CAST(Option_string)( net_dot_urllib__unescape ( s , net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component )); return opt_ok(&tmp2, sizeof(string)) ;
 
 }
 Option_string net_dot_urllib__path_unescape(string s) {
 
Option_string tmp3 = OPTION_CAST(Option_string)( net_dot_urllib__unescape ( s , net_dot_urllib__net_dot_urllib__EncodingMode_encode_path_segment )); return opt_ok(&tmp3, sizeof(string)) ;
 
 }
 Option_string net_dot_urllib__unescape(string s_, net_dot_urllib__EncodingMode mode) {
 
string s= s_ ;
 
int n= 0 ;
 
bool has_plus= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  ) { 
 
 
byte x= string_at( s , i) ;
 
 if (  x ==  '%' ) { /* case */
 
 if (string_eq( s , tos2((byte*)"") ) ) {
 
 break
 ;
 
 }
 ;
 
 n ++ ;
 
 if ( i + 2 >= s .len  ||  ! net_dot_urllib__ishex ( string_at( s , i + 1) )  ||  ! net_dot_urllib__ishex ( string_at( s , i + 2) ) ) {
 
 s  =  string_right( s , i ) ;
 
 if ( s .len > 3 ) {
 
 s  =  string_left( s , 3 ) ;
 
 }
 ;
 
return  v_error ( net_dot_urllib__error_msg ( net_dot_urllib__err_msg_escape , s ) ) ;
 
 }
 ;
 
 if ( mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_host  &&  net_dot_urllib__unhex ( string_at( s , i + 1) ) < 8  && string_ne( string_substr( s , i , i + 3 ) , tos2((byte*)"%25") ) ) {
 
return  v_error ( net_dot_urllib__error_msg ( net_dot_urllib__err_msg_escape , string_substr( s , i , i + 3 ) ) ) ;
 
 }
 ;
 
 if ( mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_zone ) {
 
byte v= ((byte)( net_dot_urllib__unhex ( string_at( s , i + 1) )  <<  ((byte)( 4 ) ) | net_dot_urllib__unhex ( string_at( s , i + 2) ) ) ) ;
 
 if (string_ne( string_substr( s , i , i + 3 ) , tos2((byte*)"%25") )  &&  v != ' '  &&  net_dot_urllib__should_escape ( v , net_dot_urllib__net_dot_urllib__EncodingMode_encode_host ) ) {
 
 v_error ( net_dot_urllib__error_msg ( net_dot_urllib__err_msg_escape , string_substr( s , i , i + 3 ) ) ) ;
 
 }
 ;
 
 }
 ;
 
 i  +=  3 ;
 
 }
 else  if (  x ==  '+' ) { /* case */
 
 has_plus  =  mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component ;
 
 i ++ ;
 
 }
 else  { // default:
 
 if ( ( mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_host  ||  mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_zone )  &&  string_at( s , i) < 0x80  &&  net_dot_urllib__should_escape ( string_at( s , i) , mode ) ) {
 
 v_error ( net_dot_urllib__error_msg ( tos2((byte*)"invalid character in host name") , string_substr( s , i , i + 1 ) ) ) ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
 }
 ;
 
 if ( n == 0  &&  ! has_plus ) {
 
string tmp26 = OPTION_CAST(string)( s); return opt_ok(&tmp26, sizeof(string)) ;
 
 }
 ;
 
strings__Builder t= strings__new_builder ( s .len - 2 * n ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
byte x= string_at( s , i) ;
 
 if (  x ==  '%' ) { /* case */
 
 strings__Builder_write(& /* ? */ t , byte_str( ((byte)( net_dot_urllib__unhex ( string_at( s , i + 1) )  <<  ((byte)( 4 ) ) | net_dot_urllib__unhex ( string_at( s , i + 2) ) ) ) ) ) ;
 
 i  +=  2 ;
 
 }
 else  if (  x ==  '+' ) { /* case */
 
 if ( mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component ) {
 
 strings__Builder_write(& /* ? */ t , tos2((byte*)" ") ) ;
 
 }
  else { 
 
 strings__Builder_write(& /* ? */ t , tos2((byte*)"+") ) ;
 
 }
 ;
 
 }
 else  { // default:
 
 strings__Builder_write(& /* ? */ t , byte_str( string_at( s , i) ) ) ;
 
 }
 ;
 
 }
 ;
 
string tmp38 = OPTION_CAST(string)( strings__Builder_str( t )); return opt_ok(&tmp38, sizeof(string)) ;
 
 }
 string net_dot_urllib__query_escape(string s) {
 
return  net_dot_urllib__escape ( s , net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component ) ;
 
 }
 string net_dot_urllib__path_escape(string s) {
 
return  net_dot_urllib__escape ( s , net_dot_urllib__net_dot_urllib__EncodingMode_encode_path_segment ) ;
 
 }
 string net_dot_urllib__escape(string s, net_dot_urllib__EncodingMode mode) {
 
int space_count= 0 ;
 
int hex_count= 0 ;
 
byte c= ((byte)( 0 ) ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 c  =  string_at( s , i) ;
 
 if ( net_dot_urllib__should_escape ( c , mode ) ) {
 
 if ( c == ' '  &&  mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component ) {
 
 space_count ++ ;
 
 }
  else { 
 
 hex_count ++ ;
 
 }
 ;
 
 }
 ;
 
 }
 ;
 
 if ( space_count == 0  &&  hex_count == 0 ) {
 
return  s ;
 
 }
 ;
 
array_byte buf= array_repeat(new_array_from_c_array(1, 1, sizeof(byte), (byte[]) {  ((byte)( 0 ) )  }) , 64 ) ;
 
array_byte t=new_array_from_c_array(0, 0, sizeof(byte), (byte[]) {   0 }) ;
 
int required= s .len + 2 * hex_count ;
 
 if ( required <= buf .len ) {
 
 t  =  array_left( buf , required ) ;
 
 }
  else { 
 
 t  =  array_repeat(new_array_from_c_array(1, 1, sizeof(byte), (byte[]) {  ((byte)( 0 ) )  }) , required ) ;
 
 }
 ;
 
 if ( hex_count == 0 ) {
 
 copy ( t , string_bytes( s ) ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( string_at( s , i) == ' ' ) {
 
array_set(&/*q*/ t , i , & (byte []) {  '+' }) ;
 
 }
 ;
 
 }
 ;
 
return  (tos2((byte *) t .data ) ) ;
 
 }
 ;
 
string ctab= tos2((byte*)"0123456789ABCDEF") ;
 
int j= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
byte c1= string_at( s , i) ;
 
 if ( c1 == ' '  &&  mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component ) {
 
array_set(&/*q*/ t , j , & (byte []) {  '+' }) ;
 
 j ++ ;
 
 }
  else  if ( net_dot_urllib__should_escape ( c1 , mode ) ) {
 
array_set(&/*q*/ t , j , & (byte []) {  '%' }) ;
 
array_set(&/*q*/ t , j + 1 , & (byte []) {  string_at( ctab , c1  >>  4) }) ;
 
array_set(&/*q*/ t , j + 2 , & (byte []) {  string_at( ctab , c1 & 15) }) ;
 
 j  +=  3 ;
 
 }
  else { 
 
array_set(&/*q*/ t , j , & (byte []) {  string_at( s , i) }) ;
 
 j ++ ;
 
 }
 ;
 
 }
 ;
 
return  (tos2((byte *) t .data ) ) ;
 
 }
 net_dot_urllib__Userinfo* net_dot_urllib__user(string username) {
 
return  (net_dot_urllib__Userinfo*)memdup(&(net_dot_urllib__Userinfo)  { .username =  username , .password =  tos2((byte*)"") , .password_set =  0 } , sizeof(net_dot_urllib__Userinfo)) ;
 
 }
 net_dot_urllib__Userinfo* net_dot_urllib__user_password(string username, string password) {
 
return  (net_dot_urllib__Userinfo*)memdup(&(net_dot_urllib__Userinfo)  { username , password , 1 } , sizeof(net_dot_urllib__Userinfo)) ;
 
 }
 bool net_dot_urllib__Userinfo_empty(net_dot_urllib__Userinfo* u) {
 
return string_eq( u ->username , tos2((byte*)"") )  && string_eq( u ->password , tos2((byte*)"") ) ;
 
 }
 string net_dot_urllib__Userinfo_string(net_dot_urllib__Userinfo* u) {
 
 if ( net_dot_urllib__Userinfo_empty(& /* ? */* u ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
string s= net_dot_urllib__escape ( u ->username , net_dot_urllib__net_dot_urllib__EncodingMode_encode_user_password ) ;
 
 if ( u ->password_set ) {
 
 s = string_add(s, string_add( tos2((byte*)":") , net_dot_urllib__escape ( u ->password , net_dot_urllib__net_dot_urllib__EncodingMode_encode_user_password ) ) ) ;
 
 }
 ;
 
return  s ;
 
 }
 Option_array_string net_dot_urllib__split_by_scheme(string rawurl) {
 
 for (
int i= 0  ;  i < rawurl .len  ;  i ++ ) { 
 
 
byte c= string_at( rawurl , i) ;
 
 if ( ( 'a' <= c  &&  c <= 'z' )  ||  ( 'A' <= c  &&  c <= 'Z' ) ) {
 
 }
  else  if ( ( '0' <= c  &&  c <= '9' )  ||  ( c == '+'  ||  c == '-'  ||  c == '.' ) ) {
 
 if ( i == 0 ) {
 
array_string tmp68 = OPTION_CAST(array_string)(new_array_from_c_array(2, 2, sizeof(string), (string[]) {  tos2((byte*)"") ,  rawurl  })); return opt_ok(&tmp68, sizeof(array_string)) ;
 
 }
 ;
 
 }
  else  if ( c == ':' ) {
 
 if ( i == 0 ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"missing protocol scheme") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
array_string tmp69 = OPTION_CAST(array_string)(new_array_from_c_array(2, 2, sizeof(string), (string[]) {  string_left( rawurl , i ) ,  string_right( rawurl , i + 1 )  })); return opt_ok(&tmp69, sizeof(array_string)) ;
 
 }
  else { 
 
array_string tmp70 = OPTION_CAST(array_string)(new_array_from_c_array(2, 2, sizeof(string), (string[]) {  tos2((byte*)"") ,  rawurl  })); return opt_ok(&tmp70, sizeof(array_string)) ;
 
 }
 ;
 
 }
 ;
 
array_string tmp71 = OPTION_CAST(array_string)(new_array_from_c_array(2, 2, sizeof(string), (string[]) {  tos2((byte*)"") ,  rawurl  })); return opt_ok(&tmp71, sizeof(array_string)) ;
 
 }
 Option_string net_dot_urllib__get_scheme(string rawurl) {
 
Option_array_string tmp72 =  net_dot_urllib__split_by_scheme ( rawurl ) ;
 if (!tmp72 .ok) {
 string err = tmp72 . error;
 
string tmp73 = OPTION_CAST(string)( err); return opt_ok(&tmp73, sizeof(string)) ;
 
 }
 array_string split = *(array_string*) tmp72 . data;
 ;
 
string tmp76 = OPTION_CAST(string)( ( *(string*) array__get( split , 0) )); return opt_ok(&tmp76, sizeof(string)) ;
 
 }
 array_string net_dot_urllib__split(string s, string c, bool cutc) {
 
int i= string_index( s , c ) ;
 
 if ( i < 0 ) {
 
return new_array_from_c_array(2, 2, sizeof(string), (string[]) {  s ,  tos2((byte*)"")  }) ;
 
 }
 ;
 
 if ( cutc ) {
 
return new_array_from_c_array(2, 2, sizeof(string), (string[]) {  string_left( s , i ) ,  string_right( s , i + c .len )  }) ;
 
 }
 ;
 
return new_array_from_c_array(2, 2, sizeof(string), (string[]) {  string_left( s , i ) ,  string_right( s , i )  }) ;
 
 }
 Option_net_dot_urllib__URL net_dot_urllib__parse(string rawurl) {
 
array_string p= net_dot_urllib__split ( rawurl , tos2((byte*)"#") , 1 ) ;
 
string u= ( *(string*) array__get( p , 0) ) ;
 
string frag= ( *(string*) array__get( p , 1) ) ;
 
Option_net_dot_urllib__URL tmp85 =  net_dot_urllib___parse ( u , 0 ) ;
 if (!tmp85 .ok) {
 string err = tmp85 . error;
 
return  v_error ( net_dot_urllib__error_msg ( net_dot_urllib__err_msg_parse , u ) ) ;
 
 }
 net_dot_urllib__URL url = *(net_dot_urllib__URL*) tmp85 . data;
 ;
 
 if (string_eq( frag , tos2((byte*)"") ) ) {
 
net_dot_urllib__URL tmp86 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp86, sizeof(net_dot_urllib__URL)) ;
 
 }
 ;
 
Option_string tmp87 =  net_dot_urllib__unescape ( frag , net_dot_urllib__net_dot_urllib__EncodingMode_encode_fragment ) ;
 if (!tmp87 .ok) {
 string err = tmp87 . error;
 
return  v_error ( net_dot_urllib__error_msg ( net_dot_urllib__err_msg_parse , u ) ) ;
 
 }
 string f = *(string*) tmp87 . data;
 ;
 
 url .fragment  =  f ;
 
net_dot_urllib__URL tmp88 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp88, sizeof(net_dot_urllib__URL)) ;
 
 }
 Option_net_dot_urllib__URL net_dot_urllib__parse_request_uri(string rawurl) {
 
Option_net_dot_urllib__URL tmp89 = OPTION_CAST(Option_net_dot_urllib__URL)( net_dot_urllib___parse ( rawurl , 1 )); return opt_ok(&tmp89, sizeof(net_dot_urllib__URL)) ;
 
 }
 Option_net_dot_urllib__URL net_dot_urllib___parse(string rawurl, bool via_request) {
 
 if ( net_dot_urllib__string_contains_ctl_byte ( rawurl ) ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"invalid control character in URL") , rawurl ) ) ;
 
 }
 ;
 
 if (string_eq( rawurl , tos2((byte*)"") )  &&  via_request ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"empty URL") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
net_dot_urllib__URL url= (net_dot_urllib__URL) { .scheme =  tos((byte *)"", 0) , .opaque =  tos((byte *)"", 0) , .user =  0 , .host =  tos((byte *)"", 0) , .path =  tos((byte *)"", 0) , .raw_path =  tos((byte *)"", 0) , .force_query =  0 , .raw_query =  tos((byte *)"", 0) , .fragment =  tos((byte *)"", 0) } ;
 
 if (string_eq( rawurl , tos2((byte*)"*") ) ) {
 
 url .path  =  tos2((byte*)"*") ;
 
net_dot_urllib__URL tmp91 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp91, sizeof(net_dot_urllib__URL)) ;
 
 }
 ;
 
Option_array_string tmp92 =  net_dot_urllib__split_by_scheme ( rawurl ) ;
 if (!tmp92 .ok) {
 string err = tmp92 . error;
 
return  v_error ( err ) ;
 
 }
 array_string p = *(array_string*) tmp92 . data;
 ;
 
 url .scheme  =  ( *(string*) array__get( p , 0) ) ;
 
string rest= ( *(string*) array__get( p , 1) ) ;
 
 url .scheme  =  string_to_lower( url .scheme ) ;
 
 if ( string_ends_with( rest , tos2((byte*)"?") )  &&  ! string_contains( string_trim_right( rest , tos2((byte*)"?") ) , tos2((byte*)"?") ) ) {
 
 url .force_query  =  1 ;
 
 rest  =  string_left( rest , rest .len - 1 ) ;
 
 }
  else { 
 
array_string parts= net_dot_urllib__split ( rest , tos2((byte*)"?") , 1 ) ;
 
 rest  =  ( *(string*) array__get( parts , 0) ) ;
 
 url .raw_query  =  ( *(string*) array__get( parts , 1) ) ;
 
 }
 ;
 
 if ( ! string_starts_with( rest , tos2((byte*)"/") ) ) {
 
 if (string_ne( url .scheme , tos2((byte*)"") ) ) {
 
 url .opaque  =  rest ;
 
net_dot_urllib__URL tmp103 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp103, sizeof(net_dot_urllib__URL)) ;
 
 }
 ;
 
 if ( via_request ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"invalid URI for request") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
int colon= string_index( rest , tos2((byte*)":") ) ;
 
int slash= string_index( rest , tos2((byte*)"/") ) ;
 
 if ( colon >= 0  &&  ( slash < 0  ||  colon < slash ) ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"first path segment in URL cannot contain colon") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
 }
 ;
 
 if ( (string_ne( url .scheme , tos2((byte*)"") )  ||  ! via_request  &&  ! string_starts_with( rest , tos2((byte*)"///") ) )  &&  string_starts_with( rest , tos2((byte*)"//") ) ) {
 
array_string parts= net_dot_urllib__split ( string_right( rest , 2 ) , tos2((byte*)"/") , 0 ) ;
 
string authority= ( *(string*) array__get( parts , 0) ) ;
 
 rest  =  ( *(string*) array__get( parts , 1) ) ;
 
Option_net_dot_urllib__ParseAuthorityRes tmp112 =  net_dot_urllib__parse_authority ( authority ) ;
 if (!tmp112 .ok) {
 string err = tmp112 . error;
 
return  v_error ( err ) ;
 
 }
 net_dot_urllib__ParseAuthorityRes a = *(net_dot_urllib__ParseAuthorityRes*) tmp112 . data;
 ;
 
 url .user  =  a .user ;
 
 url .host  =  a .host ;
 
 }
 ;
 
Option_bool tmp113 =  net_dot_urllib__URL_set_path(& /* ? */ url , rest ) ;
 if (!tmp113 .ok) {
 string err = tmp113 . error;
 
return  v_error ( err ) ;
 
 }
 bool _ = *(bool*) tmp113 . data;
 ;
 
net_dot_urllib__URL tmp114 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp114, sizeof(net_dot_urllib__URL)) ;
 
 }
 Option_net_dot_urllib__ParseAuthorityRes net_dot_urllib__parse_authority(string authority) {
 
int i= string_last_index( authority , tos2((byte*)"@") ) ;
 
string host= tos2((byte*)"") ;
 
net_dot_urllib__Userinfo* user= net_dot_urllib__user ( tos2((byte*)"") ) ;
 
 if ( i < 0 ) {
 
Option_string tmp118 =  net_dot_urllib__parse_host ( authority ) ;
 if (!tmp118 .ok) {
 string err = tmp118 . error;
 
return  v_error ( err ) ;
 
 }
 string h = *(string*) tmp118 . data;
 ;
 
 host  =  h ;
 
 }
  else { 
 
Option_string tmp119 =  net_dot_urllib__parse_host ( string_right( authority , i + 1 ) ) ;
 if (!tmp119 .ok) {
 string err = tmp119 . error;
 
return  v_error ( err ) ;
 
 }
 string h = *(string*) tmp119 . data;
 ;
 
 host  =  h ;
 
 }
 ;
 
 if ( i < 0 ) {
 
net_dot_urllib__ParseAuthorityRes tmp120 = OPTION_CAST(net_dot_urllib__ParseAuthorityRes)( (net_dot_urllib__ParseAuthorityRes) { .host =  host , .user =  0 , }); return opt_ok(&tmp120, sizeof(net_dot_urllib__ParseAuthorityRes)) ;
 
 }
 ;
 
string userinfo= string_left( authority , i ) ;
 
 if ( ! net_dot_urllib__valid_userinfo ( userinfo ) ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"invalid userinfo") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
 if ( ! string_contains( userinfo , tos2((byte*)":") ) ) {
 
Option_string tmp122 =  net_dot_urllib__unescape ( userinfo , net_dot_urllib__net_dot_urllib__EncodingMode_encode_user_password ) ;
 if (!tmp122 .ok) {
 string err = tmp122 . error;
 
return  v_error ( err ) ;
 
 }
 string u = *(string*) tmp122 . data;
 ;
 
 userinfo  =  u ;
 
 user  =  user ;
 
 ( userinfo ) ; 
 
 }
  else { 
 
array_string parts= net_dot_urllib__split ( userinfo , tos2((byte*)":") , 1 ) ;
 
string username= ( *(string*) array__get( parts , 0) ) ;
 
string password= ( *(string*) array__get( parts , 1) ) ;
 
Option_string tmp130 =  net_dot_urllib__unescape ( username , net_dot_urllib__net_dot_urllib__EncodingMode_encode_user_password ) ;
 if (!tmp130 .ok) {
 string err = tmp130 . error;
 
return  v_error ( err ) ;
 
 }
 string u = *(string*) tmp130 . data;
 ;
 
 username  =  u ;
 
Option_string tmp131 =  net_dot_urllib__unescape ( password , net_dot_urllib__net_dot_urllib__EncodingMode_encode_user_password ) ;
 if (!tmp131 .ok) {
 string err = tmp131 . error;
 
return  v_error ( err ) ;
 
 }
 string p = *(string*) tmp131 . data;
 ;
 
 password  =  p ;
 
 user  =  net_dot_urllib__user_password ( username , password ) ;
 
 }
 ;
 
net_dot_urllib__ParseAuthorityRes tmp132 = OPTION_CAST(net_dot_urllib__ParseAuthorityRes)( (net_dot_urllib__ParseAuthorityRes) { .user =  user , .host =  host }); return opt_ok(&tmp132, sizeof(net_dot_urllib__ParseAuthorityRes)) ;
 
 }
 Option_string net_dot_urllib__parse_host(string host) {
 
 if ( string_starts_with( host , tos2((byte*)"[") ) ) {
 
int i= string_last_index( host , tos2((byte*)"]") ) ;
 
 if ( i < 0 ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"missing \']\' in host") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
string colon_port= string_right( host , i + 1 ) ;
 
 if ( ! net_dot_urllib__valid_optional_port ( colon_port ) ) {
 
return  v_error ( net_dot_urllib__error_msg ( _STR("invalid port %.*s after host ", colon_port.len, colon_port.str) , tos2((byte*)"") ) ) ;
 
 }
 ;
 
int zone= string_index( string_left( host , i ) , tos2((byte*)"%25") ) ;
 
 if ( zone >= 0 ) {
 
Option_string tmp136 =  net_dot_urllib__unescape ( string_left( host , zone ) , net_dot_urllib__net_dot_urllib__EncodingMode_encode_host ) ;
 if (!tmp136 .ok) {
 string err = tmp136 . error;
 
string tmp137 = OPTION_CAST(string)( err); return opt_ok(&tmp137, sizeof(string)) ;
 
 }
 string host1 = *(string*) tmp136 . data;
 ;
 
Option_string tmp138 =  net_dot_urllib__unescape ( string_substr( host , zone , i ) , net_dot_urllib__net_dot_urllib__EncodingMode_encode_zone ) ;
 if (!tmp138 .ok) {
 string err = tmp138 . error;
 
string tmp139 = OPTION_CAST(string)( err); return opt_ok(&tmp139, sizeof(string)) ;
 
 }
 string host2 = *(string*) tmp138 . data;
 ;
 
Option_string tmp140 =  net_dot_urllib__unescape ( string_right( host , i ) , net_dot_urllib__net_dot_urllib__EncodingMode_encode_host ) ;
 if (!tmp140 .ok) {
 string err = tmp140 . error;
 
string tmp141 = OPTION_CAST(string)( err); return opt_ok(&tmp141, sizeof(string)) ;
 
 }
 string host3 = *(string*) tmp140 . data;
 ;
 
string tmp142 = OPTION_CAST(string)(string_add(string_add( host1 , host2 ) , host3 )); return opt_ok(&tmp142, sizeof(string)) ;
 
 }
  else { 
 
 i  =  string_last_index( host , tos2((byte*)":") ) ;
 
 if ( i != - 1 ) {
 
 colon_port  =  string_right( host , i ) ;
 
 if ( ! net_dot_urllib__valid_optional_port ( colon_port ) ) {
 
return  v_error ( net_dot_urllib__error_msg ( _STR("invalid port %.*s after host ", colon_port.len, colon_port.str) , tos2((byte*)"") ) ) ;
 
 }
 ;
 
 }
 ;
 
 }
 ;
 
 }
 ;
 
Option_string tmp143 =  net_dot_urllib__unescape ( host , net_dot_urllib__net_dot_urllib__EncodingMode_encode_host ) ;
 if (!tmp143 .ok) {
 string err = tmp143 . error;
 
string tmp144 = OPTION_CAST(string)( err); return opt_ok(&tmp144, sizeof(string)) ;
 
 }
 string h = *(string*) tmp143 . data;
 ;
 
string tmp145 = OPTION_CAST(string)( h); return opt_ok(&tmp145, sizeof(string)) ;
 
 }
 Option_bool net_dot_urllib__URL_set_path(net_dot_urllib__URL* u, string p) {
 
Option_string tmp146 =  net_dot_urllib__unescape ( p , net_dot_urllib__net_dot_urllib__EncodingMode_encode_path ) ;
 if (!tmp146 .ok) {
 string err = tmp146 . error;
 
return  v_error ( err ) ;
 
 }
 string path = *(string*) tmp146 . data;
 ;
 
 u ->path  =  path ;
 
string escp= net_dot_urllib__escape ( path , net_dot_urllib__net_dot_urllib__EncodingMode_encode_path ) ;
 
 if (string_eq( p , escp ) ) {
 
 u ->raw_path  =  tos2((byte*)"") ;
 
 }
  else { 
 
 u ->raw_path  =  p ;
 
 }
 ;
 
bool tmp148 = OPTION_CAST(bool)( 1); return opt_ok(&tmp148, sizeof(bool)) ;
 
 }
 string net_dot_urllib__URL_escaped_path(net_dot_urllib__URL* u) {
 
 if (string_ne( u ->raw_path , tos2((byte*)"") )  &&  net_dot_urllib__valid_encoded_path ( u ->raw_path ) ) {
 
Option_string p= net_dot_urllib__unescape ( u ->raw_path , net_dot_urllib__net_dot_urllib__EncodingMode_encode_path ) ;
 
return  u ->raw_path ;
 
 }
 ;
 
 if (string_eq( u ->path , tos2((byte*)"*") ) ) {
 
return  tos2((byte*)"*") ;
 
 }
 ;
 
return  net_dot_urllib__escape ( u ->path , net_dot_urllib__net_dot_urllib__EncodingMode_encode_path ) ;
 
 }
 bool net_dot_urllib__valid_encoded_path(string s) {
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
byte x= string_at( s , i) ;
 
 if (  x ==  '!'  ||   x ==  '$'  ||   x ==  '&'  ||   x ==  '\\'  ||   x ==  '('  ||   x ==  ')'  ||   x ==  '*'  ||   x ==  '+'  ||   x ==  ','  ||   x ==  ';'  ||   x ==  '='  ||   x ==  ':'  ||   x ==  '@' ) { /* case */
 
 }
 else  if (  x ==  '['  ||   x ==  ']' ) { /* case */
 
 }
 else  if (  x ==  '%' ) { /* case */
 
 }
 else  { // default:
 
 if ( net_dot_urllib__should_escape ( string_at( s , i) , net_dot_urllib__net_dot_urllib__EncodingMode_encode_path ) ) {
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
 }
 ;
 
return  1 ;
 
 }
 bool net_dot_urllib__valid_optional_port(string port) {
 
 if (string_eq( port , tos2((byte*)"") ) ) {
 
return  1 ;
 
 }
 ;
 
 if ( string_at( port , 0) != ':' ) {
 
return  0 ;
 
 }
 ;
 
 string tmp158 =  string_right( port , 1 );
 array_byte bytes_tmp158 = string_bytes( tmp158 );
 ;
for (int tmp159 = 0; tmp159 < tmp158 .len; tmp159 ++) {
 byte b = ((byte *) bytes_tmp158 . data)[tmp159];
 
 
 if ( b < '0'  ||  b > '9' ) {
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
return  1 ;
 
 }
 string net_dot_urllib__URL_str(net_dot_urllib__URL* u) {
 
strings__Builder buf= strings__new_builder ( 200 ) ;
 
 if (string_ne( u ->scheme , tos2((byte*)"") ) ) {
 
 strings__Builder_write(& /* ? */ buf , u ->scheme ) ;
 
 strings__Builder_write(& /* ? */ buf , tos2((byte*)":") ) ;
 
 }
 ;
 
 if (string_ne( u ->opaque , tos2((byte*)"") ) ) {
 
 strings__Builder_write(& /* ? */ buf , u ->opaque ) ;
 
 }
  else { 
 
 if (string_ne( u ->scheme , tos2((byte*)"") )  || string_ne( u ->host , tos2((byte*)"") )  ||  ! net_dot_urllib__Userinfo_empty(& /* ? */* u ->user ) ) {
 
 if (string_ne( u ->host , tos2((byte*)"") )  || string_ne( u ->path , tos2((byte*)"") )  ||  ! net_dot_urllib__Userinfo_empty(& /* ? */* u ->user ) ) {
 
 strings__Builder_write(& /* ? */ buf , tos2((byte*)"//") ) ;
 
 }
 ;
 
 if ( ! net_dot_urllib__Userinfo_empty(& /* ? */* u ->user ) ) {
 
 strings__Builder_write(& /* ? */ buf , net_dot_urllib__Userinfo_string(& /* ? */* u ->user ) ) ;
 
 strings__Builder_write(& /* ? */ buf , tos2((byte*)"@") ) ;
 
 }
 ;
 
 if (string_ne( u ->host , tos2((byte*)"") ) ) {
 
 strings__Builder_write(& /* ? */ buf , net_dot_urllib__escape ( u ->host , net_dot_urllib__net_dot_urllib__EncodingMode_encode_host ) ) ;
 
 }
 ;
 
 }
 ;
 
string path= net_dot_urllib__URL_escaped_path(& /* ? */* u ) ;
 
 if (string_ne( path , tos2((byte*)"") )  &&  string_at( path , 0) != '/'  && string_ne( u ->host , tos2((byte*)"") ) ) {
 
 strings__Builder_write(& /* ? */ buf , tos2((byte*)"/") ) ;
 
 }
 ;
 
 if ( buf .len == 0 ) {
 
int i= string_index( path , tos2((byte*)":") ) ;
 
 if ( i > - 1  &&  string_index( string_left( path , i ) , tos2((byte*)"/") ) == - 1 ) {
 
 strings__Builder_write(& /* ? */ buf , tos2((byte*)"./") ) ;
 
 }
 ;
 
 }
 ;
 
 strings__Builder_write(& /* ? */ buf , path ) ;
 
 }
 ;
 
 if ( u ->force_query  || string_ne( u ->raw_query , tos2((byte*)"") ) ) {
 
 strings__Builder_write(& /* ? */ buf , tos2((byte*)"?") ) ;
 
 strings__Builder_write(& /* ? */ buf , u ->raw_query ) ;
 
 }
 ;
 
 if (string_ne( u ->fragment , tos2((byte*)"") ) ) {
 
 strings__Builder_write(& /* ? */ buf , tos2((byte*)"#") ) ;
 
 strings__Builder_write(& /* ? */ buf , net_dot_urllib__escape ( u ->fragment , net_dot_urllib__net_dot_urllib__EncodingMode_encode_fragment ) ) ;
 
 }
 ;
 
return  strings__Builder_str( buf ) ;
 
 }
 Option_net_dot_urllib__Values net_dot_urllib__parse_query(string query) {
 
net_dot_urllib__Values m= net_dot_urllib__new_values ( ) ;
 
Option_bool tmp166 =  net_dot_urllib___parse_query (& /*112 EXP:"net_dot_urllib__Values*" GOT:"net_dot_urllib__Values" */ m , query ) ;
 if (!tmp166 .ok) {
 string err = tmp166 . error;
 
return  v_error ( err ) ;
 
 }
 bool _ = *(bool*) tmp166 . data;
 ;
 
net_dot_urllib__Values tmp167 = OPTION_CAST(net_dot_urllib__Values)( m); return opt_ok(&tmp167, sizeof(net_dot_urllib__Values)) ;
 
 }
 net_dot_urllib__Values net_dot_urllib__parse_query_silent(string query) {
 
net_dot_urllib__Values m= net_dot_urllib__new_values ( ) ;
 
Option_bool _= net_dot_urllib___parse_query (& /*112 EXP:"net_dot_urllib__Values*" GOT:"net_dot_urllib__Values" */ m , query ) ;
 
return  m ;
 
 }
 Option_bool net_dot_urllib___parse_query(net_dot_urllib__Values* m, string query) {
 
bool had_error= 0 ;
 
string q= query ;
 
 while (string_ne( q , tos2((byte*)"") ) ) {
 
 
string key= q ;
 
int i= string_index_any( key , tos2((byte*)"&;") ) ;
 
 if ( i >= 0 ) {
 
 q  =  string_right( key , i + 1 ) ;
 
 key  =  string_left( key , i ) ;
 
 }
  else { 
 
 q  =  tos2((byte*)"") ;
 
 }
 ;
 
 if (string_eq( key , tos2((byte*)"") ) ) {
 
 continue
 ;
 
 }
 ;
 
string value= tos2((byte*)"") ;
 
 i  =  string_index( key , tos2((byte*)"=") ) ;
 
 if ( i >= 0 ) {
 
 value  =  string_right( key , i + 1 ) ;
 
 key  =  string_left( key , i ) ;
 
 }
 ;
 
Option_string tmp175 =  net_dot_urllib__query_unescape ( key ) ;
 if (!tmp175 .ok) {
 string err = tmp175 . error;
 
 had_error  =  1 ;
 
 continue
 ;
 
 }
 string k = *(string*) tmp175 . data;
 ;
 
 key  =  k ;
 
Option_string tmp176 =  net_dot_urllib__query_unescape ( value ) ;
 if (!tmp176 .ok) {
 string err = tmp176 . error;
 
 had_error  =  1 ;
 
 continue
 ;
 
 }
 string v = *(string*) tmp176 . data;
 ;
 
 value  =  v ;
 
 net_dot_urllib__Values_add( m , key , value ) ;
 
 }
 ;
 
 if ( had_error ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"error parsing query string") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
bool tmp177 = OPTION_CAST(bool)( 1); return opt_ok(&tmp177, sizeof(bool)) ;
 
 }
 string net_dot_urllib__Values_encode(net_dot_urllib__Values v) {
 
 if ( v .size == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
strings__Builder buf= strings__new_builder ( 200 ) ;
 
array_string keys=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 map_Value tmp180 =  v .data;
 array_string keys_tmp180 = map_keys(& tmp180 ); 
 for (int l = 0; l < keys_tmp180 .len; l++) {
 string k = ((string*)keys_tmp180 .data)[l];
 Value _ = {0}; map_get(tmp180, k, & _);
 
 
_PUSH(& keys , ( k ), tmp181, string) ;
 
 }
 ;
 
 array_string_sort(& /* ? */ keys ) ;
 
 array_string tmp182 =  keys;
 for (int tmp183 = 0; tmp183 < tmp182.len; tmp183++) {
 string k = ((string *) tmp182 . data)[tmp183];
 
  
 Value tmp184 = {0}; bool tmp185 = map_get( v .data , k, & tmp184); 

Value vs= tmp184 ;
 
string key_kscaped= net_dot_urllib__query_escape ( k ) ;
 
 array_string tmp188 =  vs .data;
 for (int _ = 0; _ < tmp188.len; _++) {
 string val = ((string *) tmp188 . data)[_];
 
 
 if ( buf .len > 0 ) {
 
 strings__Builder_write(& /* ? */ buf , tos2((byte*)"&") ) ;
 
 }
 ;
 
 strings__Builder_write(& /* ? */ buf , key_kscaped ) ;
 
 strings__Builder_write(& /* ? */ buf , tos2((byte*)"=") ) ;
 
 strings__Builder_write(& /* ? */ buf , net_dot_urllib__query_escape ( val ) ) ;
 
 }
 ;
 
 }
 ;
 
return  strings__Builder_str( buf ) ;
 
 }
 string net_dot_urllib__resolve_path(string base, string ref) {
 
string full= tos2((byte*)"") ;
 
 if (string_eq( ref , tos2((byte*)"") ) ) {
 
 full  =  base ;
 
 }
  else  if ( string_at( ref , 0) != '/' ) {
 
int i= string_last_index( base , tos2((byte*)"/") ) ;
 
 full  = string_add( string_left( base , i + 1 ) , ref ) ;
 
 }
  else { 
 
 full  =  ref ;
 
 }
 ;
 
 if (string_eq( full , tos2((byte*)"") ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
array_string dst=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
array_string src= string_split( full , tos2((byte*)"/") ) ;
 
 array_string tmp195 =  src;
 for (int _ = 0; _ < tmp195.len; _++) {
 string elem = ((string *) tmp195 . data)[_];
 
 
 if ( string_eq( elem,  tos2((byte*)".") ) ) { /* case */
 
 }
 else  if ( string_eq( elem,  tos2((byte*)"..") ) ) { /* case */
 
 if ( dst .len > 0 ) {
 
 dst  =  array_left( dst , dst .len - 1 ) ;
 
 }
 ;
 
 }
 else  { // default:
 
_PUSH(& dst , ( elem ), tmp196, string) ;
 
 }
 ;
 
 }
 ;
 
string last= ( *(string*) array__get( src , src .len - 1) ) ;
 
 if (string_eq( last , tos2((byte*)".") )  || string_eq( last , tos2((byte*)"..") ) ) {
 
_PUSH(& dst , ( tos2((byte*)"") ), tmp200, string) ;
 
 }
 ;
 
return string_add( tos2((byte*)"/") , string_trim_left( array_string_join( dst , tos2((byte*)"/") ) , tos2((byte*)"/") ) ) ;
 
 }
 bool net_dot_urllib__URL_is_abs(net_dot_urllib__URL* u) {
 
return string_ne( u ->scheme , tos2((byte*)"") ) ;
 
 }
 Option_net_dot_urllib__URL net_dot_urllib__URL_parse(net_dot_urllib__URL* u, string ref) {
 
Option_net_dot_urllib__URL tmp201 =  net_dot_urllib__parse ( ref ) ;
 if (!tmp201 .ok) {
 string err = tmp201 . error;
 
return  v_error ( err ) ;
 
 }
 net_dot_urllib__URL refurl = *(net_dot_urllib__URL*) tmp201 . data;
 ;
 
Option_net_dot_urllib__URL tmp202 = OPTION_CAST(Option_net_dot_urllib__URL)( net_dot_urllib__URL_resolve_reference(& /* ? */* u ,& /*112 EXP:"net_dot_urllib__URL*" GOT:"net_dot_urllib__URL" */ refurl )); return opt_ok(&tmp202, sizeof(net_dot_urllib__URL)) ;
 
 }
 Option_net_dot_urllib__URL net_dot_urllib__URL_resolve_reference(net_dot_urllib__URL* u, net_dot_urllib__URL* ref) {
 
net_dot_urllib__URL url= * ref ;
 
 if (string_eq( ref ->scheme , tos2((byte*)"") ) ) {
 
 url .scheme  =  u ->scheme ;
 
 }
 ;
 
 if (string_ne( ref ->scheme , tos2((byte*)"") )  || string_ne( ref ->host , tos2((byte*)"") )  ||  ! net_dot_urllib__Userinfo_empty(& /* ? */* ref ->user ) ) {
 
 net_dot_urllib__URL_set_path(& /* ? */ url , net_dot_urllib__resolve_path ( net_dot_urllib__URL_escaped_path(& /* ? */* ref ) , tos2((byte*)"") ) ) ;
 
net_dot_urllib__URL tmp204 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp204, sizeof(net_dot_urllib__URL)) ;
 
 }
 ;
 
 if (string_ne( ref ->opaque , tos2((byte*)"") ) ) {
 
 url .user  =  net_dot_urllib__user ( tos2((byte*)"") ) ;
 
 url .host  =  tos2((byte*)"") ;
 
 url .path  =  tos2((byte*)"") ;
 
net_dot_urllib__URL tmp205 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp205, sizeof(net_dot_urllib__URL)) ;
 
 }
 ;
 
 if (string_eq( ref ->path , tos2((byte*)"") )  && string_eq( ref ->raw_query , tos2((byte*)"") ) ) {
 
 url .raw_query  =  u ->raw_query ;
 
 if (string_eq( ref ->fragment , tos2((byte*)"") ) ) {
 
 url .fragment  =  u ->fragment ;
 
 }
 ;
 
 }
 ;
 
 url .host  =  u ->host ;
 
 url .user  =  u ->user ;
 
 net_dot_urllib__URL_set_path(& /* ? */ url , net_dot_urllib__resolve_path ( net_dot_urllib__URL_escaped_path(& /* ? */* u ) , net_dot_urllib__URL_escaped_path(& /* ? */* ref ) ) ) ;
 
net_dot_urllib__URL tmp206 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp206, sizeof(net_dot_urllib__URL)) ;
 
 }
 net_dot_urllib__Values net_dot_urllib__URL_query(net_dot_urllib__URL* u) {
 
net_dot_urllib__Values v= net_dot_urllib__parse_query_silent ( u ->raw_query ) ;
 
return  v ;
 
 }
 string net_dot_urllib__URL_request_uri(net_dot_urllib__URL* u) {
 
string result= u ->opaque ;
 
 if (string_eq( result , tos2((byte*)"") ) ) {
 
 result  =  net_dot_urllib__URL_escaped_path(& /* ? */* u ) ;
 
 if (string_eq( result , tos2((byte*)"") ) ) {
 
 result  =  tos2((byte*)"/") ;
 
 }
 ;
 
 }
  else { 
 
 if ( string_starts_with( result , tos2((byte*)"//") ) ) {
 
 result  = string_add(string_add( u ->scheme , tos2((byte*)":") ) , result ) ;
 
 }
 ;
 
 }
 ;
 
 if ( u ->force_query  || string_ne( u ->raw_query , tos2((byte*)"") ) ) {
 
 result = string_add(result, string_add( tos2((byte*)"?") , u ->raw_query ) ) ;
 
 }
 ;
 
return  result ;
 
 }
 string net_dot_urllib__URL_hostname(net_dot_urllib__URL* u) {
 
array_string host_port= net_dot_urllib__split_host_port ( u ->host ) ;
 
return  ( *(string*) array__get( host_port , 0) ) ;
 
 }
 string net_dot_urllib__URL_port(net_dot_urllib__URL* u) {
 
array_string host_port= net_dot_urllib__split_host_port ( u ->host ) ;
 
return  ( *(string*) array__get( host_port , 1) ) ;
 
 }
 array_string net_dot_urllib__split_host_port(string hostport) {
 
string host= hostport ;
 
string port= tos2((byte*)"") ;
 
int colon= string_last_index( host , tos2((byte*)":") ) ;
 
 if ( colon != - 1  &&  net_dot_urllib__valid_optional_port ( string_right( host , colon ) ) ) {
 
 port  =  string_right( host , colon + 1 ) ;
 
 host  =  string_left( host , colon ) ;
 
 }
 ;
 
 if ( string_starts_with( host , tos2((byte*)"[") )  &&  string_ends_with( host , tos2((byte*)"]") ) ) {
 
 host  =  string_substr( host , 1 , host .len - 1 ) ;
 
 }
 ;
 
return new_array_from_c_array(2, 2, sizeof(string), (string[]) {  host ,  port  }) ;
 
 }
 bool net_dot_urllib__valid_userinfo(string s) {
 
 string tmp218 =  s;
 array_byte bytes_tmp218 = string_bytes( tmp218 );
 ;
for (int tmp219 = 0; tmp219 < tmp218 .len; tmp219 ++) {
 byte r = ((byte *) bytes_tmp218 . data)[tmp219];
 
 
 if ( 'A' <= r  &&  r <= 'Z' ) {
 
 continue
 ;
 
 }
 ;
 
 if ( 'a' <= r  &&  r <= 'z' ) {
 
 continue
 ;
 
 }
 ;
 
 if ( '0' <= r  &&  r <= '9' ) {
 
 continue
 ;
 
 }
 ;
 
 if (  r ==  '-'  ||   r ==  '.'  ||   r ==  '_'  ||   r ==  ':'  ||   r ==  '~'  ||   r ==  '!'  ||   r ==  '$'  ||   r ==  '&'  ||   r ==  '\\'  ||   r ==  '('  ||   r ==  ')'  ||   r ==  '*'  ||   r ==  '+'  ||   r ==  ','  ||   r ==  ';'  ||   r ==  '='  ||   r ==  '%'  ||   r ==  '@' ) { /* case */
 
 continue
 ;
 
 }
 else  { // default:
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
return  1 ;
 
 }
 bool net_dot_urllib__string_contains_ctl_byte(string s) {
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
byte b= string_at( s , i) ;
 
 if ( b < ' '  ||  b == 0x7f ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 }
 bool net_dot_urllib__ishex(byte c) {
 
 if ( '0' <= c  &&  c <= '9' ) {
 
return  1 ;
 
 }
  else  if ( 'a' <= c  &&  c <= 'f' ) {
 
return  1 ;
 
 }
  else  if ( 'A' <= c  &&  c <= 'F' ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 }
 byte net_dot_urllib__unhex(byte c) {
 
 if ( '0' <= c  &&  c <= '9' ) {
 
return  c - '0' ;
 
 }
  else  if ( 'a' <= c  &&  c <= 'f' ) {
 
return  c - 'a' + 10 ;
 
 }
  else  if ( 'A' <= c  &&  c <= 'F' ) {
 
return  c - 'A' + 10 ;
 
 }
 ;
 
return  0 ;
 
 }
 net_dot_urllib__Values net_dot_urllib__new_values() {
 
return  (net_dot_urllib__Values) { .data =  new_map(1, sizeof(Value)) , .size =  0 } ;
 
 }
 array_string Value_all(Value* v) {
 
return  v ->data ;
 
 }
 string net_dot_urllib__Values_get(net_dot_urllib__Values v, string key) {
 
 if ( v .data .size == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
  
 Value tmp1 = {0}; bool tmp2 = map_get( v .data , key, & tmp1); 

Value vs= tmp1 ;
 
 if ( vs .data .len == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  ( *(string*) array__get( vs .data , 0) ) ;
 
 }
 array_string net_dot_urllib__Values_get_all(net_dot_urllib__Values v, string key) {
 
 if ( v .data .size == 0 ) {
 
return new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 }
 ;
  
 Value tmp6 = {0}; bool tmp7 = map_get( v .data , key, & tmp6); 

Value vs= tmp6 ;
 
 if ( vs .data .len == 0 ) {
 
return new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 }
 ;
 
return  vs .data ;
 
 }
 void net_dot_urllib__Values_set(net_dot_urllib__Values* v, string key, string value) {
  
 Value tmp9 = {0}; bool tmp10 = map_get( v ->data , key, & tmp9); 

Value a= tmp9 ;
 
 a .data  = new_array_from_c_array(1, 1, sizeof(string), (string[]) {  value  }) ;
 
map__set(& v ->data , key , & (Value []) {  a }) ;
 
 v ->size  =  v ->data .size ;
 
 }
 void net_dot_urllib__Values_add(net_dot_urllib__Values* v, string key, string value) {
  
 Value tmp12 = {0}; bool tmp13 = map_get( v ->data , key, & tmp12); 

Value a= tmp12 ;
 
 if ( a .data .len == 0 ) {
 
 a .data  = new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 }
 ;
 
_PUSH(& a .data , ( value ), tmp15, string) ;
 
map__set(& v ->data , key , & (Value []) {  a }) ;
 
 v ->size  =  v ->data .size ;
 
 }
 void net_dot_urllib__Values_del(net_dot_urllib__Values* v, string key) {
 
 v_map_delete(& /* ? */ v ->data , key ) ;
 
 v ->size  =  v ->data .size ;
 
 }
 void rand__seed(int s) {
 
 srand ( s ) ;
 
 }
 int rand__next(int max) {
 
return  rand ( ) % max ;
 
 }
 int rand__rand_r(int* seed) {
 
int* rs= seed ;
 
int ns= ( * rs * 1103515245 + 12345 ) ;
 
 * rs  =  ns ; 
 
return  ns & 0x7fffffff ;
 
 }
 void http__init_module() {
 
 #ifdef __APPLE__
 
 SSL_library_init ( ) ;
 
 #endif
 ;
 
 #ifdef __linux__
 
 SSL_library_init ( ) ;
 
 #endif
 ;
 
 }
 http__Response http__Request_ssl_do(http__Request* req, int port, string method, string host_name, string path) {
 
void* ssl_method= TLSv1_2_method ( ) ;
 
 if ( isnil (& /*112 EXP:"void*" GOT:"string" */ method ) ) {
 
 }
 ;
 
void* ctx= SSL_CTX_new ( ssl_method ) ;
 
 if ( isnil ( ctx ) ) {
 
 }
 ;
 
 SSL_CTX_set_verify_depth ( ctx ,  4 ) ;
 
int flags= SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION ;
 
 SSL_CTX_set_options ( ctx ,  flags ) ;
 
void* res= SSL_CTX_load_verify_locations ( ctx ,  "random-org-chain.pem" ,  0 ) ;
 
 if ( res != 1 ) {
 
 }
 ;
 
void* web= BIO_new_ssl_connect ( ctx ) ;
 
 if ( isnil ( ctx ) ) {
 
 }
 ;
 
string addr=string_add(string_add( host_name , tos2((byte*)":") ) , int_str( port ) ) ;
 
 res  =  BIO_set_conn_hostname ( web , (char*) addr .str ) ;
 
 if ( res != 1 ) {
 
 }
 ;
  
 struct /*c struct init*/ 

SSL* ssl= 0 ;
 
 BIO_get_ssl ( web ,  & /*v*/  ssl ) ;
 
 if ( isnil ( ssl ) ) {
 
 }
 ;
 
string preferred_ciphers= tos2((byte*)"HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4") ;
 
 res  =  SSL_set_cipher_list ( ssl , (char*) preferred_ciphers .str ) ;
 
 if ( res != 1 ) {
 
 }
 ;
 
 res  =  SSL_set_tlsext_host_name ( ssl , (char*) host_name .str ) ;
 
 res  =  BIO_do_connect ( web ) ;
 
 res  =  BIO_do_handshake ( web ) ;
 
void* cert= SSL_get_peer_certificate ( ssl ) ;
 
 res  =  SSL_get_verify_result ( ssl ) ;
 
string s= http__Request_build_request_headers(& /* ? */* req , method , host_name , path ) ;
 
 BIO_puts ( web , (char*) s .str ) ;
 
strings__Builder sb= strings__new_builder ( 100 ) ;
 
 while (1) { 
 
byte buff  [1536 ] ;
 
int len= ((int)( BIO_read ( web ,  buff ,  1536 ) ) ) ;
 
 if ( len > 0 ) {
 
 strings__Builder_write(& /* ? */ sb , tos ( buff , len ) ) ;
 
 }
  else { 
 
 break
 ;
 
 }
 ;
 
 }
 ;
 
 if ( ! isnil ( web ) ) {
 
 BIO_free_all ( web ) ;
 
 }
 ;
 
 if ( ! isnil ( ctx ) ) {
 
 SSL_CTX_free ( ctx ) ;
 
 }
 ;
 
return  http__parse_response ( strings__Builder_str( sb ) ) ;
 
 }
 void http__download_cb(void* ptr, size_t size, size_t nmemb, void* userp) {
 
 }
 void http__download_file_with_progress(string url, string out, downloadfn cb, void (*cb_finished)(  /*FFF*/ )) {
 
 }
 void http__download_file(string url, string out) {
 
 }
 void http__empty() {
 
 }
 Option_http__Response http__get(string url) {
 
Option_http__Request tmp1 =  http__new_request ( tos2((byte*)"GET") , url , tos2((byte*)"") ) ;
 if (!tmp1 .ok) {
 string err = tmp1 . error;
 
return  v_error ( err ) ;
 
 }
 http__Request req = *(http__Request*) tmp1 . data;
 ;
 
Option_http__Response tmp2 = OPTION_CAST(Option_http__Response)( http__Request_do(& /* ? */ req )); return opt_ok(&tmp2, sizeof(http__Response)) ;
 
 }
 Option_http__Response http__post(string url, string data) {
 
Option_http__Request tmp3 =  http__new_request ( tos2((byte*)"POST") , url , data ) ;
 if (!tmp3 .ok) {
 string err = tmp3 . error;
 
return  v_error ( err ) ;
 
 }
 http__Request req = *(http__Request*) tmp3 . data;
 ;
 
Option_http__Response tmp4 = OPTION_CAST(Option_http__Response)( http__Request_do(& /* ? */ req )); return opt_ok(&tmp4, sizeof(http__Response)) ;
 
 }
 Option_http__Request http__new_request(string typ, string _url, string _data) {
 
 if (string_eq( _url , tos2((byte*)"") ) ) {
 
return  v_error ( tos2((byte*)"bad url") ) ;
 
 }
 ;
 
string url= _url ;
 
string data= _data ;
 
 if (string_eq( typ , tos2((byte*)"GET") )  &&  ! string_contains( url , tos2((byte*)"?") )  && string_ne( data , tos2((byte*)"") ) ) {
 
 url  =  _STR("%.*s?%.*s", url.len, url.str, data.len, data.str) ;
 
 data  =  tos2((byte*)"") ;
 
 }
 ;
 
http__Request tmp7 = OPTION_CAST(http__Request)( (http__Request) { .typ =  typ , .url =  url , .data =  data , .ws_func =  0 , .user_ptr =  0 , .headers =  new_map(1, sizeof(string)) , .user_agent =  tos2((byte*)"v") , .method =  tos((byte *)"", 0) , .h =  tos((byte *)"", 0) , .cmd =  tos((byte *)"", 0) , .verbose =  0 , }); return opt_ok(&tmp7, sizeof(http__Request)) ;
 
 }
 string http__get_text(string url) {
 
Option_http__Response tmp8 =  http__get ( url ) ;
 if (!tmp8 .ok) {
 string err = tmp8 . error;
 
return  tos2((byte*)"") ;
 
 }
 http__Response resp = *(http__Response*) tmp8 . data;
 ;
 
return  resp .text ;
 
 }
 void http__Request_free(http__Request* req) {
 
 v_map_free(& /* ? */ req ->headers ) ;
 
 }
 void http__Response_free(http__Response* resp) {
 
 v_map_free(& /* ? */ resp ->headers ) ;
 
 }
 void http__Request_add_header(http__Request* req, string key, string val) {
 
map__set(& req ->headers , key , & (string []) {  val }) ;
 
 }
 map_string http__parse_headers(array_string lines) {
 
map_string headers= new_map(1, sizeof(string)) ;
 
 array_string tmp10 =  lines;
 for (int i = 0; i < tmp10.len; i++) {
 string line = ((string *) tmp10 . data)[i];
 
 
 if ( i == 0 ) {
 
 continue
 ;
 
 }
 ;
 
array_string words= string_split( line , tos2((byte*)": ") ) ;
 
 if ( words .len != 2 ) {
 
 continue
 ;
 
 }
 ;
 
map__set(& headers , ( *(string*) array__get( words , 0) ) , & (string []) {  ( *(string*) array__get( words , 1) ) }) ;
 
 }
 ;
 
return  headers ;
 
 }
 Option_http__Response http__Request_do(http__Request* req) {
 
 if (string_eq( req ->typ , tos2((byte*)"POST") ) ) {
 
 }
 ;
 
 map_string tmp16 =  req ->headers;
 array_string keys_tmp16 = map_keys(& tmp16 ); 
 for (int l = 0; l < keys_tmp16 .len; l++) {
 string key = ((string*)keys_tmp16 .data)[l];
 string val = {0}; map_get(tmp16, key, & val);
 
 
 }
 ;
 
Option_net_dot_urllib__URL tmp17 =  net_dot_urllib__parse ( req ->url ) ;
 if (!tmp17 .ok) {
 string err = tmp17 . error;
 
return  v_error ( _STR("http.request.do: invalid URL %.*s", req ->url.len, req ->url.str) ) ;
 
 }
 net_dot_urllib__URL url = *(net_dot_urllib__URL*) tmp17 . data;
 ;
 
net_dot_urllib__URL rurl= url ;
 
http__Response resp= (http__Response) { .text =  tos((byte *)"", 0) , .headers =  new_map(1, sizeof(string)) , .status_code =  0 } ;
 
int no_redirects= 0 ;
 
 while (1) { 
 
 if ( no_redirects == http__max_redirects ) {
 
return  v_error ( _STR("http.request.do: maximum number of redirects reached (%d)", http__max_redirects) ) ;
 
 }
 ;
 
Option_http__Response tmp21 =  http__Request_method_and_url_to_response(& /* ? */* req , req ->typ , rurl ) ;
 if (!tmp21 .ok) {
 string err = tmp21 . error;
 
return  v_error ( err ) ;
 
 }
 http__Response qresp = *(http__Response*) tmp21 . data;
 ;
 
 resp  =  qresp ;
 
 if ( ! (_IN(int, ( resp .status_code ), new_array_from_c_array(5, 5, sizeof(int), (int[]) {  301 ,  302 ,  303 ,  307 ,  308  }) ) ) ) {
 
 break
 ;
 
 }
 ;
  
 string tmp22 = tos((byte *)"", 0); bool tmp23 = map_get( resp .headers , tos2((byte*)"Location"), & tmp22); 
 
 if (!tmp23) tmp22 = tos((byte *)"", 0); 

string redirect_url= tmp22 ;
 
Option_net_dot_urllib__URL tmp25 =  net_dot_urllib__parse ( redirect_url ) ;
 if (!tmp25 .ok) {
 string err = tmp25 . error;
 
return  v_error ( _STR("http.request.do: invalid URL in redirect %.*s", redirect_url.len, redirect_url.str) ) ;
 
 }
 net_dot_urllib__URL qrurl = *(net_dot_urllib__URL*) tmp25 . data;
 ;
 
 rurl  =  qrurl ;
 
 no_redirects ++ ;
 
 }
 ;
 
http__Response tmp26 = OPTION_CAST(http__Response)( resp); return opt_ok(&tmp26, sizeof(http__Response)) ;
 
 }
 Option_http__Response http__Request_method_and_url_to_response(http__Request* req, string method, net_dot_urllib__URL url) {
 
string host_name= net_dot_urllib__URL_hostname(& /* ? */ url ) ;
 
string scheme= url .scheme ;
 
string p= string_trim_left( url .path , tos2((byte*)"/") ) ;
 
string path= ( net_dot_urllib__URL_query(& /* ? */ url ) .size > 0 ) ? ( _STR("/%.*s?%.*s", p.len, p.str, net_dot_urllib__Values_encode( net_dot_urllib__URL_query(& /* ? */ url ) ).len, net_dot_urllib__Values_encode( net_dot_urllib__URL_query(& /* ? */ url ) ).str) ) : ( _STR("/%.*s", p.len, p.str) ) ;
 
int nport= v_string_int( net_dot_urllib__URL_port(& /* ? */ url ) ) ;
 
 if ( nport == 0 ) {
 
 if (string_eq( scheme , tos2((byte*)"http") ) ) {
 
 nport  =  80 ;
 
 }
 ;
 
 if (string_eq( scheme , tos2((byte*)"https") ) ) {
 
 nport  =  443 ;
 
 }
 ;
 
 }
 ;
 
 if (string_eq( scheme , tos2((byte*)"https") ) ) {
 
http__Response tmp32 = OPTION_CAST(http__Response)( http__Request_ssl_do(& /* ? */* req , nport , method , host_name , path )); return opt_ok(&tmp32, sizeof(http__Response)) ;
 
 }
  else  if (string_eq( scheme , tos2((byte*)"http") ) ) {
 
Option_http__Response tmp33 = OPTION_CAST(Option_http__Response)( http__Request_http_do(& /* ? */* req , nport , method , host_name , path )); return opt_ok(&tmp33, sizeof(http__Response)) ;
 
 }
 ;
 
return  v_error ( _STR("http.request.do: unsupported scheme: %.*s", scheme.len, scheme.str) ) ;
 
 }
 http__Response http__parse_response(string resp) {
 
map_string headers= new_map(1, sizeof(string)) ;
 
string first_header= string_all_before( resp , tos2((byte*)"\n") ) ;
 
int status_code= 0 ;
 
 if ( string_contains( first_header , tos2((byte*)"HTTP/") ) ) {
 
string val= string_find_between( first_header , tos2((byte*)" ") , tos2((byte*)" ") ) ;
 
 status_code  =  v_string_int( val ) ;
 
 }
 ;
 
string text= tos2((byte*)"") ;
 
int nl_pos= 3 ;
 
int i= 1 ;
 
 while (1) { 
 
int old_pos= nl_pos ;
 
 nl_pos  =  string_index_after( resp , tos2((byte*)"\n") , nl_pos + 1 ) ;
 
 if ( nl_pos == - 1 ) {
 
 break
 ;
 
 }
 ;
 
string h= string_substr( resp , old_pos + 1 , nl_pos ) ;
 
 if ( h .len <= 1 ) {
 
 text  =  string_right( resp , nl_pos + 1 ) ;
 
 break
 ;
 
 }
 ;
 
 i ++ ;
 
int pos= string_index( h , tos2((byte*)":") ) ;
 
 if ( pos == - 1 ) {
 
 continue
 ;
 
 }
 ;
 
string key= string_left( h , pos ) ;
 
string val= string_right( h , pos + 2 ) ;
 
map__set(& headers , key , & (string []) {  string_trim_space( val ) }) ;
 
 }
 ;
  
 string tmp46 = tos((byte *)"", 0); bool tmp47 = map_get( headers , tos2((byte*)"Transfer-Encoding"), & tmp46); 
 
 if (!tmp47) tmp46 = tos((byte *)"", 0); 

 if (string_eq( tmp46 , tos2((byte*)"chunked") ) ) {
 
 text  =  http_dot_chunked__decode ( text ) ;
 
 }
 ;
 
return  (http__Response) { .status_code =  status_code , .headers =  headers , .text =  text } ;
 
 }
 string http__Request_build_request_headers(http__Request* req, string method, string host_name, string path) {
 
string ua= req ->user_agent ;
 
array_string uheaders=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 map_string tmp50 =  req ->headers;
 array_string keys_tmp50 = map_keys(& tmp50 ); 
 for (int l = 0; l < keys_tmp50 .len; l++) {
 string key = ((string*)keys_tmp50 .data)[l];
 string val = {0}; map_get(tmp50, key, & val);
 
 
_PUSH(& uheaders , ( _STR("%.*s: %.*s\r\n", key.len, key.str, val.len, val.str) ), tmp51, string) ;
 
 }
 ;
 
 if ( req ->data .len > 0 ) {
 
_PUSH(& uheaders , ( _STR("Content-Length: %d\r\n", req ->data .len) ), tmp52, string) ;
 
 }
 ;
 
return string_add(string_add(string_add(string_add(string_add( _STR("%.*s %.*s HTTP/1.1\r\n", method.len, method.str, path.len, path.str) , _STR("Host: %.*s\r\n", host_name.len, host_name.str) ) , _STR("User-Agent: %.*s\r\n", ua.len, ua.str) ) , array_string_join( uheaders , tos2((byte*)"") ) ) , tos2((byte*)"Connection: close\r\n\r\n") ) , req ->data ) ;
 
 }
 string http__unescape_url(string s) {
 
 v_panic ( tos2((byte*)"http.unescape_url() was replaced with urllib.query_unescape()") ) ;
 
 }
 string http__escape_url(string s) {
 
 v_panic ( tos2((byte*)"http.escape_url() was replaced with urllib.query_escape()") ) ;
 
 }
 string http__unescape(string s) {
 
 v_panic ( tos2((byte*)"http.unescape() was replaced with http.unescape_url()") ) ;
 
 }
 string http__escape(string s) {
 
 v_panic ( tos2((byte*)"http.escape() was replaced with http.escape_url()") ) ;
 
 }
 Option_http__Response http__Request_http_do(http__Request* req, int port, string method, string host_name, string path) {
 
int bufsize= 512 ;
 
byte rbuffer  [512 ] ;
 
strings__Builder sb= strings__new_builder ( 100 ) ;
 
string s= http__Request_build_request_headers(& /* ? */* req , method , host_name , path ) ;
 
Option_net__Socket tmp5 =  net__dial ( host_name , port ) ;
 if (!tmp5 .ok) {
 string err = tmp5 . error;
 
return  v_error ( err ) ;
 
 }
 net__Socket client = *(net__Socket*) tmp5 . data;
 ;
 
 net__Socket_send( client , s .str , s .len ) ;
 
 while (1) { 
 
int readbytes= net__Socket_crecv( client , rbuffer , bufsize ) ;
 
 if ( readbytes < 0 ) {
 
return  v_error ( _STR("http_do error reading response. readbytes: %d", readbytes) ) ;
 
 }
 ;
 
 if ( readbytes == 0 ) {
 
 break
 ;
 
 }
 ;
 
 strings__Builder_write(& /* ? */ sb , tos ( rbuffer , readbytes ) ) ;
 
 }
 ;
 
 net__Socket_close( client ) ;
 
http__Response tmp7 = OPTION_CAST(http__Response)( http__parse_response ( strings__Builder_str( sb ) )); return opt_ok(&tmp7, sizeof(http__Response)) ;
 
 }
 int http_dot_chunked__ChunkScanner_read_chunk_size(http_dot_chunked__ChunkScanner* s) {
 
int n= 0 ;
 
 while (1) { 
 
 if ( s ->pos >= s ->text .len ) {
 
 break
 ;
 
 }
 ;
 
byte c= string_at( s ->text , s ->pos) ;
 
 if ( ! byte_is_hex_digit( c ) ) {
 
 break
 ;
 
 }
 ;
 
 n  =  n  <<  4 ;
 
 n  +=  ((int)( http_dot_chunked__unhex ( c ) ) ) ;
 
 s ->pos ++ ;
 
 }
 ;
 
return  n ;
 
 }
 byte http_dot_chunked__unhex(byte c) {
 
 if ( '0' <= c  &&  c <= '9' ) {
 
return  c - '0' ;
 
 }
  else  if ( 'a' <= c  &&  c <= 'f' ) {
 
return  c - 'a' + 10 ;
 
 }
  else  if ( 'A' <= c  &&  c <= 'F' ) {
 
return  c - 'A' + 10 ;
 
 }
 ;
 
return  0 ;
 
 }
 void http_dot_chunked__ChunkScanner_skip_crlf(http_dot_chunked__ChunkScanner* s) {
 
 s ->pos  +=  2 ;
 
 }
 string http_dot_chunked__ChunkScanner_read_chunk(http_dot_chunked__ChunkScanner* s, int chunksize) {
 
int startpos= s ->pos ;
 
 s ->pos  +=  chunksize ;
 
return  string_substr( s ->text , startpos , s ->pos ) ;
 
 }
 string http_dot_chunked__decode(string text) {
 
strings__Builder sb= strings__new_builder ( 100 ) ;
 
http_dot_chunked__ChunkScanner cscanner= (http_dot_chunked__ChunkScanner) { .pos =  0 , .text =  text } ;
 
 while (1) { 
 
int csize= http_dot_chunked__ChunkScanner_read_chunk_size(& /* ? */ cscanner ) ;
 
 if ( 0 == csize ) {
 
 break
 ;
 
 }
 ;
 
 http_dot_chunked__ChunkScanner_skip_crlf(& /* ? */ cscanner ) ;
 
 strings__Builder_write(& /* ? */ sb , http_dot_chunked__ChunkScanner_read_chunk(& /* ? */ cscanner , csize ) ) ;
 
 http_dot_chunked__ChunkScanner_skip_crlf(& /* ? */ cscanner ) ;
 
 }
 ;
 
 http_dot_chunked__ChunkScanner_skip_crlf(& /* ? */ cscanner ) ;
 
return  strings__Builder_str( sb ) ;
 
 }
 void time__remove_me_when_c_bug_is_fixed() {
 
 }
 time__Time time__now() {
 
i64 t= time ( 0 ) ;
  
 struct /*c struct init*/ 

tm* now= 0 ;
 
 now  =  localtime ( & /*v*/  t ) ;
 
return  time__convert_ctime (* now ) ;
 
 }
 time__Time time__random() {
 
int now_unix= time__now ( ) .uni ;
 
int rand_unix= rand__next ( now_unix ) ;
 
return  time__unix ( rand_unix ) ;
 
 }
 time__Time time__unix(int abs) {
 
int d= abs / time__secondsPerDay ;
 
int n= d / time__daysPer400Years ;
 
int y= 400 * n ;
 
 d  -=  time__daysPer400Years * n ;
 
 n  =  d / time__daysPer100Years ;
 
 n  -=  n  >>  2 ;
 
 y  +=  100 * n ;
 
 d  -=  time__daysPer100Years * n ;
 
 n  =  d / time__daysPer4Years ;
 
 y  +=  4 * n ;
 
 d  -=  time__daysPer4Years * n ;
 
 n  =  d / 365 ;
 
 n  -=  n  >>  2 ;
 
 y  +=  n ;
 
 d  -=  365 * n ;
 
int yday= ((int)( d ) ) ;
 
int day= yday ;
 
int year= abs / ((int)( 3.154e+7 ) ) + 1970 ;
 
int hour= ((int)( abs % time__secondsPerDay ) ) / time__secondsPerHour ;
 
int minute= ((int)( abs % time__secondsPerHour ) ) / time__secondsPerMinute ;
 
int second= ((int)( abs % time__secondsPerMinute ) ) ;
 
 if ( time__is_leap_year ( year ) ) {
 
 if ( day > 31 + 29 - 1 ) {
 
 day -- ;
 
 }
  else  if ( day == 31 + 29 - 1 ) {
 
 day  =  29 ;
 
return  (time__Time) { .year =  year , .month =  2 , .day =  day , .hour =  hour , .minute =  minute , .second =  second , .uni =  0 } ;
 
 }
 ;
 
 }
 ;
 
int month= day / 31 ;
 
int begin= 0 ;
 
int end= ((int)( ( *(int*) array__get( time__daysBefore , month + 1) ) ) ) ;
 
 if ( day >= end ) {
 
 month ++ ;
 
 begin  =  end ;
 
 }
  else { 
 
 begin  =  ((int)( ( *(int*) array__get( time__daysBefore , month) ) ) ) ;
 
 }
 ;
 
 month ++ ;
 
 day  =  day - begin + 1 ;
 
return  (time__Time) { .year =  year , .month =  month , .day =  day , .hour =  hour , .minute =  minute , .second =  second , .uni =  0 } ;
 
 }
 time__Time time__convert_ctime(struct /*TM*/ tm t) {
 
return  (time__Time) { .year =  t .tm_year + 1900 , .month =  t .tm_mon + 1 , .day =  t .tm_mday , .hour =  t .tm_hour , .minute =  t .tm_min , .second =  t .tm_sec , .uni =  mktime ( & /*v*/  t ) } ;
 
 }
 string time__Time_format_ss(time__Time t) {
 
return  _STR("%d-%02d-%02d %02d:%02d:%02d", t .year, t .month, t .day, t .hour, t .minute, t .second) ;
 
 }
 string time__Time_format(time__Time t) {
 
return  _STR("%d-%02d-%02d %02d:%02d", t .year, t .month, t .day, t .hour, t .minute) ;
 
 }
 string time__Time_smonth(time__Time t) {
 
int i= t .month - 1 ;
 
return  string_substr( time__Months , i * 3 , ( i + 1 ) * 3 ) ;
 
 }
 string time__Time_hhmm(time__Time t) {
 
return  _STR("%02d:%02d", t .hour, t .minute) ;
 
 }
 string time__Time_hhmm12(time__Time t) {
 
string am= tos2((byte*)"am") ;
 
int hour= t .hour ;
 
 if ( t .hour > 11 ) {
 
 am  =  tos2((byte*)"pm") ;
 
 }
 ;
 
 if ( t .hour > 12 ) {
 
 hour  =  hour - 12 ;
 
 }
 ;
 
 if ( t .hour == 0 ) {
 
 hour  =  12 ;
 
 }
 ;
 
return  _STR("%d:%02d %.*s", hour, t .minute, am.len, am.str) ;
 
 }
 string time__Time_hhmmss(time__Time t) {
 
return  _STR("%02d:%02d:%02d", t .hour, t .minute, t .second) ;
 
 }
 string time__Time_ymmdd(time__Time t) {
 
return  _STR("%d-%02d-%02d", t .year, t .month, t .day) ;
 
 }
 string time__Time_md(time__Time t) {
 
string s= _STR("%.*s %d", time__Time_smonth( t ).len, time__Time_smonth( t ).str, t .day) ;
 
return  s ;
 
 }
 string time__Time_clean(time__Time t) {
 
time__Time nowe= time__now ( ) ;
 
 if ( t .month == nowe .month  &&  t .year == nowe .year  &&  t .day == nowe .day ) {
 
return  time__Time_hhmm( t ) ;
 
 }
 ;
 
 if ( t .year == nowe .year ) {
 
return  _STR("%.*s %d %.*s", time__Time_smonth( t ).len, time__Time_smonth( t ).str, t .day, time__Time_hhmm( t ).len, time__Time_hhmm( t ).str) ;
 
 }
 ;
 
return  time__Time_format( t ) ;
 
 }
 string time__Time_clean12(time__Time t) {
 
time__Time nowe= time__now ( ) ;
 
 if ( t .month == nowe .month  &&  t .year == nowe .year  &&  t .day == nowe .day ) {
 
return  time__Time_hhmm12( t ) ;
 
 }
 ;
 
 if ( t .year == nowe .year ) {
 
return  _STR("%.*s %d %.*s", time__Time_smonth( t ).len, time__Time_smonth( t ).str, t .day, time__Time_hhmm12( t ).len, time__Time_hhmm12( t ).str) ;
 
 }
 ;
 
return  time__Time_format( t ) ;
 
 }
 time__Time time__parse(string s) {
 
int pos= string_index( s , tos2((byte*)" ") ) ;
 
 if ( pos <= 0 ) {
 
 println ( tos2((byte*)"bad time format") ) ;
 
return  time__now ( ) ;
 
 }
 ;
 
string symd= string_left( s , pos ) ;
 
array_string ymd= string_split( symd , tos2((byte*)"-") ) ;
 
 if ( ymd .len != 3 ) {
 
 println ( tos2((byte*)"bad time format") ) ;
 
return  time__now ( ) ;
 
 }
 ;
 
string shms= string_right( s , pos ) ;
 
array_string hms= string_split( shms , tos2((byte*)":") ) ;
 
string hour= ( *(string*) array__get( hms , 0) ) ;
 
string minute= ( *(string*) array__get( hms , 1) ) ;
 
string second= ( *(string*) array__get( hms , 2) ) ;
 
return  time__new_time ( (time__Time) { .year =  v_string_int( ( *(string*) array__get( ymd , 0) ) ) , .month =  v_string_int( ( *(string*) array__get( ymd , 1) ) ) , .day =  v_string_int( ( *(string*) array__get( ymd , 2) ) ) , .hour =  v_string_int( hour ) , .minute =  v_string_int( minute ) , .second =  v_string_int( second ) , .uni =  0 } ) ;
 
 }
 time__Time time__new_time(time__Time t) {
 
return  (time__Time){ .uni =  time__Time_calc_unix(& /* ? */ t ) , .year = t . year, .month = t . month, .day = t . day, .hour = t . hour, .minute = t . minute, .second = t . second, } ;
 
 }
 int time__Time_calc_unix(time__Time* t) {
 
 if ( t ->uni != 0 ) {
 
return  t ->uni ;
 
 }
 ;

struct /*TM*/ tm tt= (struct tm) { .tm_sec =  t ->second , .tm_min =  t ->minute , .tm_hour =  t ->hour , .tm_mday =  t ->day , .tm_mon =  t ->month - 1 , .tm_year =  t ->year - 1900 } ;
 
return  mktime ( & /*v*/  tt ) ;
 
 }
 time__Time time__Time_add_seconds(time__Time t, int seconds) {
 
return  time__unix ( t .uni + seconds ) ;
 
 }
 int time__since(time__Time t) {
 
return  0 ;
 
 }
 string time__Time_relative(time__Time t) {
 
time__Time now= time__now ( ) ;
 
int secs= now .uni - t .uni ;
 
 if ( secs <= 30 ) {
 
return  tos2((byte*)"now") ;
 
 }
 ;
 
 if ( secs < 60 ) {
 
return  tos2((byte*)"1m") ;
 
 }
 ;
 
 if ( secs < 3600 ) {
 
return  _STR("%dm", secs / 60) ;
 
 }
 ;
 
 if ( secs < 3600 * 24 ) {
 
return  _STR("%dh", secs / 3600) ;
 
 }
 ;
 
 if ( secs < 3600 * 24 * 5 ) {
 
return  _STR("%dd", secs / 3600 / 24) ;
 
 }
 ;
 
 if ( secs > 3600 * 24 * 10000 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  time__Time_md( t ) ;
 
 }
 int time__day_of_week(int y, int m, int d) {
 
array_int t=new_array_from_c_array(12, 12, sizeof(int), (int[]) {  0 ,  3 ,  2 ,  5 ,  0 ,  3 ,  5 ,  1 ,  4 ,  6 ,  2 ,  4  }) ;
 
int sy= y ;
 
 if ( ( m < 3 ) ) {
 
 sy  =  sy - 1 ;
 
 }
 ;
 
return  ( sy + sy / 4 - sy / 100 + sy / 400 + ( *(int*) array__get( t , m - 1) ) + d - 1 ) % 7 + 1 ;
 
 }
 int time__Time_day_of_week(time__Time t) {
 
return  time__day_of_week ( t .year , t .month , t .day ) ;
 
 }
 string time__Time_weekday_str(time__Time t) {
 
int i= time__Time_day_of_week( t ) - 1 ;
 
return  string_substr( time__Days , i * 3 , ( i + 1 ) * 3 ) ;
 
 }
 i64 time__ticks() {
 
 #ifdef _WIN32
 
return  GetTickCount ( ) ;
 
 #else
  
 struct /*c struct init*/ 

timeval ts ;
 
 gettimeofday ( & /*v*/  ts ,  0 ) ;
 
return  ts .tv_sec * 1000 + ( ts .tv_usec / 1000 ) ;
 
 #endif
 ;
 
 }
 void time__sleep(int seconds) {
 
 #ifdef _WIN32
 
 _sleep ( seconds * 1000 ) ;
 
 #else
 
 sleep ( seconds ) ;
 
 #endif
 ;
 
 }
 void time__usleep(int n) {
 
 #ifdef _WIN32
 
 #else
 
 usleep ( n ) ;
 
 #endif
 ;
 
 }
 void time__sleep_ms(int n) {
 
 #ifdef _WIN32
 
 Sleep ( n ) ;
 
 #else
 
 usleep ( n * 1000 ) ;
 
 #endif
 ;
 
 }
 bool time__is_leap_year(int year) {
 
return  ( year % 4 == 0 )  &&  ( year % 100 != 0  ||  year % 400 == 0 ) ;
 
 }
 Option_int time__days_in_month(int month, int year) {
 
 if ( month > 12  ||  month < 1 ) {
 
return  v_error ( _STR("Invalid month: %d", month) ) ;
 
 }
 ;
 
int extra= ( month == 2  &&  time__is_leap_year ( year ) ) ? ( 1 ) : ( 0 ) ;
 
int res= ( *(int*) array__get( time__MonthDays , month - 1) ) + extra ;
 
int tmp60 = OPTION_CAST(int)( res); return opt_ok(&tmp60, sizeof(int)) ;
 
 }
 pg__DB pg__connect(pg__Config config) {
 
string conninfo= _STR("host=%.*s user=%.*s dbname=%.*s", config .host.len, config .host.str, config .user.len, config .user.str, config .dbname.len, config .dbname.str) ;
 
PGconn* conn= PQconnectdb ((char*) conninfo .str ) ;
 
int status= PQstatus ( conn ) ;
 
 if ( status != CONNECTION_OK ) {
 
byte* error_msg= PQerrorMessage ( conn ) ;
 
 eprintln (string_add( tos2((byte*)"Connection to a PG database failed: ") , (tos2((byte *) error_msg ) ) ) ) ;
 
 v_exit ( 1 ) ;
 
 }
 ;
 
return  (pg__DB) { .conn =  conn } ;
 
 }
 array_pg__Row pg__res_to_rows(void* res) {
 
void* nr_rows= PQntuples ( res ) ;
 
void* nr_cols= PQnfields ( res ) ;
 
array_pg__Row rows=new_array_from_c_array(0, 0, sizeof(pg__Row), (pg__Row[]) {   0 }) ;
 
 for (
int i= 0  ;  i < nr_rows  ;  i ++ ) { 
 
 
pg__Row row= (pg__Row) { .vals =  new_array(0, 1, sizeof( string )) } ;
 
 for (
int j= 0  ;  j < nr_cols  ;  j ++ ) { 
 
 
byte* val= PQgetvalue ( res ,  i ,  j ) ;
 
_PUSH(& row .vals , ( (tos2((byte *) val ) ) ), tmp12, string) ;
 
 }
 ;
 
_PUSH(& rows , ( row ), tmp13, pg__Row) ;
 
 }
 ;
 
return  rows ;
 
 }
 int pg__DB_q_int(pg__DB db, string query) {
 
array_pg__Row rows= pg__DB_exec( db , query ) ;
 
 if ( rows .len == 0 ) {
 
printf( "q_int \"%.*s\" not found\n", query.len, query.str ) ;
 
return  0 ;
 
 }
 ;
 
pg__Row row= ( *(pg__Row*) array__get( rows , 0) ) ;
 
 if ( row .vals .len == 0 ) {
 
return  0 ;
 
 }
 ;
 
string val= ( *(string*) array__get( row .vals , 0) ) ;
 
return  v_string_int( val ) ;
 
 }
 string pg__DB_q_string(pg__DB db, string query) {
 
array_pg__Row rows= pg__DB_exec( db , query ) ;
 
 if ( rows .len == 0 ) {
 
printf( "q_string \"%.*s\" not found\n", query.len, query.str ) ;
 
return  tos2((byte*)"") ;
 
 }
 ;
 
pg__Row row= ( *(pg__Row*) array__get( rows , 0) ) ;
 
 if ( row .vals .len == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
string val= ( *(string*) array__get( row .vals , 0) ) ;
 
return  val ;
 
 }
 array_pg__Row pg__DB_q_strings(pg__DB db, string query) {
 
return  pg__DB_exec( db , query ) ;
 
 }
 array_pg__Row pg__DB_exec(pg__DB db, string query) {
 
void* res= PQexec ( db .conn , (char*) query .str ) ;
 
string e= (tos2((byte *) PQerrorMessage ( db .conn ) ) ) ;
 
 if (string_ne( e , tos2((byte*)"") ) ) {
 
 println ( tos2((byte*)"pg exec error:") ) ;
 
 println ( e ) ;
 
return  pg__res_to_rows ( res ) ;
 
 }
 ;
 
return  pg__res_to_rows ( res ) ;
 
 }
 Option_pg__Row pg__rows_first_or_empty(array_pg__Row rows) {
 
 if ( rows .len == 0 ) {
 
return  v_error ( tos2((byte*)"no row") ) ;
 
 }
 ;
 
pg__Row tmp32 = OPTION_CAST(pg__Row)( ( *(pg__Row*) array__get( rows , 0) )); return opt_ok(&tmp32, sizeof(pg__Row)) ;
 
 }
 Option_pg__Row pg__DB_exec_one(pg__DB db, string query) {
 
void* res= PQexec ( db .conn , (char*) query .str ) ;
 
string e= (tos2((byte *) PQerrorMessage ( db .conn ) ) ) ;
 
 if (string_ne( e , tos2((byte*)"") ) ) {
 
return  v_error ( _STR("pg exec error: \"%.*s\"", e.len, e.str) ) ;
 
 }
 ;
 
Option_pg__Row row= pg__rows_first_or_empty ( pg__res_to_rows ( res ) ) ;
 
Option_pg__Row tmp36 = OPTION_CAST(Option_pg__Row)( row); return opt_ok(&tmp36, sizeof(pg__Row)) ;
 
 }
 array_pg__Row pg__DB_exec_param2(pg__DB db, string query, string param, string param2) {
 
byte* param_vals  [2 ] ;
 
 param_vals [ 0 ]/*rbyte* 1*/  =  param .str ;
 
 param_vals [ 1 ]/*rbyte* 1*/  =  param2 .str ;
 
void* res= PQexecParams ( db .conn , (char*) query .str ,  2 ,  0 ,  param_vals ,  0 ,  0 ,  0 ) ;
 
string e= (tos2((byte *) PQerrorMessage ( db .conn ) ) ) ;
 
 if (string_ne( e , tos2((byte*)"") ) ) {
 
 println ( tos2((byte*)"pg exec2 error:") ) ;
 
 println ( e ) ;
 
return  pg__res_to_rows ( res ) ;
 
 }
 ;
 
return  pg__res_to_rows ( res ) ;
 
 }
 array_pg__Row pg__DB_exec_param(pg__DB db, string query, string param) {
 
byte* param_vals  [1 ] ;
 
 param_vals [ 0 ]/*rbyte* 1*/  =  param .str ;
 
void* res= PQexecParams ( db .conn , (char*) query .str ,  1 ,  0 ,  param_vals ,  0 ,  0 ,  0 ) ;
 
return  pg__res_to_rows ( res ) ;
 
 }
 void vweb__Context_html(vweb__Context ctx, string html) {
 
 net__Socket_write( ctx .conn , _STR("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n%.*s\r\n\r\n%.*s", ctx .headers.len, ctx .headers.str, html.len, html.str) ) ;
 
 }
 void vweb__Context_text(vweb__Context ctx, string s) {
 
 net__Socket_write( ctx .conn , _STR("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n%.*s\r\n\r\n %.*s", ctx .headers.len, ctx .headers.str, s.len, s.str) ) ;
 
 }
 void vweb__Context_json(vweb__Context ctx, string s) {
 
 net__Socket_write( ctx .conn , _STR("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n%.*s\r\n\r\n%.*s", ctx .headers.len, ctx .headers.str, s.len, s.str) ) ;
 
 }
 void vweb__Context_redirect(vweb__Context ctx, string url) {
 
 net__Socket_write( ctx .conn , _STR("HTTP/1.1 302 Found\r\nLocation: %.*s\r\n\r\n%.*s", url.len, url.str, ctx .headers.len, ctx .headers.str) ) ;
 
 }
 void vweb__Context_not_found(vweb__Context ctx, string s) {
 
 net__Socket_write( ctx .conn , vweb__HTTP_404 ) ;
 
 }
 void vweb__Context_set_cookie(vweb__Context* ctx, string key, string val) {
 
 vweb__Context_add_header( ctx , tos2((byte*)"Set-Cookie") , _STR("%.*s=%.*s", key.len, key.str, val.len, val.str) ) ;
 
 }
 Option_string vweb__Context_get_cookie(vweb__Context* ctx, string key) {
 
string cookie_header= vweb__Context_get_header( ctx , tos2((byte*)"Cookie") ) ;
 
string cookie= ( string_contains( cookie_header , tos2((byte*)";") ) ) ? ( string_find_between( cookie_header , _STR("%.*s=", key.len, key.str) , tos2((byte*)";") ) ) : ( cookie_header ) ;
 
 if (string_ne( cookie , tos2((byte*)"") ) ) {
 
string tmp3 = OPTION_CAST(string)( cookie); return opt_ok(&tmp3, sizeof(string)) ;
 
 }
 ;
 
return  v_error ( tos2((byte*)"Cookie not found") ) ;
 
 }
 void vweb__Context_add_header(vweb__Context* ctx, string key, string val) {
 
 ctx ->headers  = string_add( ctx ->headers , (string_eq( ctx ->headers , tos2((byte*)"") ) ) ? ( _STR("%.*s: val", key.len, key.str) ) : ( _STR("\r\n%.*s: val", key.len, key.str) ) ) ;
 
 }
 string vweb__Context_get_header(vweb__Context* ctx, string key) {
 
return  string_find_between( ctx ->headers , _STR("\r\n%.*s: ", key.len, key.str) , tos2((byte*)"\r\n") ) ;
 
 }
 void vweb__run_App(int port) {
 // T start 2 println
 
printf( "Running vweb app on http://localhost:%d ...\n", port ) ;
 
Option_net__Socket tmp4 =  net__listen ( port ) ;
 if (!tmp4 .ok) {
 string err = tmp4 . error;
 
 v_panic ( tos2((byte*)"failed to listen") ) ;
 
 }
 net__Socket l = *(net__Socket*) tmp4 . data;
 ;
 
App app= (App) { EMPTY_STRUCT_INITIALIZATION } ;
 
 App_init(& /* ? */ app ) ;
 
 while (1) { 
 
Option_net__Socket tmp6 =  net__Socket_accept( l ) ;
 if (!tmp6 .ok) {
 string err = tmp6 . error;
 
 v_panic ( tos2((byte*)"accept() failed") ) ;
 
 }
 net__Socket conn = *(net__Socket*) tmp6 . data;
 ;
 
string s= net__Socket_read_line( conn ) ;
 
 if (string_eq( s , tos2((byte*)"") ) ) {
 
 net__Socket_write( conn , vweb__HTTP_500 ) ;
 
 net__Socket_close( conn ) ;
 
 return ;
 
 }
 ;
 
string first_line= string_all_before( s , tos2((byte*)"\n") ) ;
 
array_string vals= string_split( first_line , tos2((byte*)" ") ) ;
 
 if ( vals .len < 2 ) {
 
 println ( tos2((byte*)"no vals for http") ) ;
 
 net__Socket_write( conn , vweb__HTTP_500 ) ;
 
 net__Socket_close( conn ) ;
 
 return ;
 
 }
 ;
 
string action= string_all_before( string_right( ( *(string*) array__get( vals , 1) ) , 1 ) , tos2((byte*)"/") ) ;
 
 if ( string_contains( action , tos2((byte*)"?") ) ) {
 
 action  =  string_all_before( action , tos2((byte*)"?") ) ;
 
 }
 ;
 
 if (string_eq( action , tos2((byte*)"") ) ) {
 
 action  =  tos2((byte*)"index") ;
 
 }
 ;
 
http__Request req= (http__Request) { .headers =  http__parse_headers ( string_split_into_lines( s ) ) , .ws_func =  0 , .user_ptr =  0 , .method =  ( *(string*) array__get( vals , 0) ) , .url =  ( *(string*) array__get( vals , 1) ) , .h =  tos((byte *)"", 0) , .cmd =  tos((byte *)"", 0) , .typ =  tos((byte *)"", 0) , .data =  tos((byte *)"", 0) , .verbose =  0 , .user_agent =  tos((byte *)"", 0) } ;
 
 #ifdef VDEBUG
 
printf( "vweb action = \"%.*s\"\n", action.len, action.str ) ;
 
 #endif
 ;
 
 app .vweb  =  (vweb__Context) { .req =  req , .conn =  conn , .form =  new_map(1, sizeof(string)) , .static_files =  new_map(1, sizeof(string)) , .static_mime_types =  new_map(1, sizeof(string)) , .headers =  tos((byte *)"", 0) } ;
 
 if (_IN(string, ( req .method ),  vweb__methods_with_form ) ) {
 
 vweb__Context_parse_form(& /* ? */ app .vweb , s ) ;
 
 }
 ;
 
 if ( vals .len < 2 ) {
 
 #ifdef VDEBUG
 
 println ( tos2((byte*)"no vals for http") ) ;
 
 #endif
 ;
 
 net__Socket_close( conn ) ;
 
 continue
 ;
 
 }
 ;
 
 if ( string_eq(action, _STR("init")) ) App_init(& app); else {
 
 net__Socket_write( conn , vweb__HTTP_404 ) ;
 
 }
 ;
 
 net__Socket_close( conn ) ;
 
 }
 ;
 
 }
 void vweb__Context_parse_form(vweb__Context* ctx, string s) {
 
 if ( ! (_IN(string, ( ctx ->req .method ),  vweb__methods_with_form ) ) ) {
 
 return ;
 
 }
 ;
 
int pos= string_index( s , tos2((byte*)"\r\n\r\n") ) ;
 
 if ( pos > - 1 ) {
 
string str_form= string_substr( s , pos , s .len ) ;
 
 str_form  =  string_replace( str_form , tos2((byte*)"+") , tos2((byte*)" ") ) ;
 
array_string words= string_split( str_form , tos2((byte*)"&") ) ;
 
 array_string tmp35 =  words;
 for (int tmp36 = 0; tmp36 < tmp35.len; tmp36++) {
 string word = ((string *) tmp35 . data)[tmp36];
 
 
 #ifdef VDEBUG
 
printf( "parse form keyval=\"%.*s\"\n", word.len, word.str ) ;
 
 #endif
 ;
 
array_string keyval= string_split( string_trim_space( word ) , tos2((byte*)"=") ) ;
 
 if ( keyval .len != 2 ) {
 
 continue
 ;
 
 }
 ;
 
string key= ( *(string*) array__get( keyval , 0) ) ;
 
Option_string tmp43 =  net_dot_urllib__query_unescape ( ( *(string*) array__get( keyval , 1) ) ) ;
 if (!tmp43 .ok) {
 string err = tmp43 . error;
 
 continue
 ;
 
 }
 string val = *(string*) tmp43 . data;
 ;
 
 #ifdef VDEBUG
 
printf( "http form \"%.*s\" => \"%.*s\"\n", key.len, key.str, val.len, val.str ) ;
 
 #endif
 ;
 
map__set(& ctx ->form , key , & (string []) {  val }) ;
 
 }
 ;
 
 }
 ;
 
 }
 void vweb__Context_scan_static_directory(vweb__Context* ctx, string directory_path, string mount_path) {
 
array_string files= os__ls ( directory_path ) ;
 
 if ( files .len > 0 ) {
 
 array_string tmp45 =  files;
 for (int tmp46 = 0; tmp46 < tmp45.len; tmp46++) {
 string file = ((string *) tmp45 . data)[tmp46];
 
 
string ext= tos2((byte*)"") ;
 
int i= file .len ;
 
bool flag= 1 ;
 
 while ( i > 0 ) {
 
 
 i -- ;
 
 if ( flag ) {
 
 ext  = string_add( string_substr( file , i , i + 1 ) , ext ) ;
 
 }
 ;
 
 if (string_eq( string_substr( file , i , i + 1 ) , tos2((byte*)".") ) ) {
 
 flag  =  0 ;
 
 }
 ;
 
 }
 ;
 
 if ( flag ) {
 
 vweb__Context_scan_static_directory( ctx ,string_add(string_add( directory_path , tos2((byte*)"/") ) , file ) ,string_add(string_add( mount_path , tos2((byte*)"/") ) , file ) ) ;
 
 }
  else { 
 
map__set(& ctx ->static_files ,string_add(string_add( mount_path , tos2((byte*)"/") ) , file ) , & (string []) { string_add(string_add( directory_path , tos2((byte*)"/") ) , file ) }) ;
  
 string tmp50 = tos((byte *)"", 0); bool tmp51 = map_get( vweb__mime_types , ext, & tmp50); 
 
 if (!tmp51) tmp50 = tos((byte *)"", 0); 

map__set(& ctx ->static_mime_types ,string_add(string_add( mount_path , tos2((byte*)"/") ) , file ) , & (string []) {  tmp50 }) ;
 
 }
 ;
 
 }
 ;
 
 }
 ;
 
 }
 bool vweb__Context_handle_static(vweb__Context* ctx, string directory_path) {
 
 vweb__Context_scan_static_directory( ctx , directory_path , tos2((byte*)"") ) ;
  
 string tmp52 = tos((byte *)"", 0); bool tmp53 = map_get( ctx ->static_files , ctx ->req .url, & tmp52); 
 
 if (!tmp53) tmp52 = tos((byte *)"", 0); 

string static_file= tmp52 ;
  
 string tmp55 = tos((byte *)"", 0); bool tmp56 = map_get( ctx ->static_mime_types , ctx ->req .url, & tmp55); 
 
 if (!tmp56) tmp55 = tos((byte *)"", 0); 

string mime_type= tmp55 ;
 
 if (string_ne( static_file , tos2((byte*)"") ) ) {
 
Option_string tmp58 =  os__read_file ( static_file ) ;
 if (!tmp58 .ok) {
 string err = tmp58 . error;
 
return  0 ;
 
 }
 string data = *(string*) tmp58 . data;
 ;
 
 net__Socket_write( ctx ->conn , _STR("HTTP/1.1 200 OK\r\nContent-Type: %.*s\r\n\r\n%.*s", mime_type.len, mime_type.str, data.len, data.str) ) ;
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 }
 void vweb__Context_serve_static(vweb__Context* ctx, string url, string file_path, string mime_type) {
 
map__set(& ctx ->static_files , url , & (string []) {  file_path }) ;
 
map__set(& ctx ->static_mime_types , url , & (string []) {  mime_type }) ;
 
 }
 int main(int argc, char** argv) {
 init_consts();
 os__args = os__init_os_args(argc, (byteptr*)argv);
 
printf( "Running vorum on http://0.0.0.0:%d\n", main__port ) ;
 
 vweb__run_App ( main__port ) ;
 
 }
 void App_init(App* app) {
 
