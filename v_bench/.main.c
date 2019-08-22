 

#include <stdio.h>  // TODO remove all these includes, define all function signatures and types manually
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h> // for va_list
#include <inttypes.h>  // int64_t etc
#include <string.h> // memcpy

#define STRUCT_DEFAULT_VALUE {}
#define EMPTY_STRUCT_DECLARATION
#define EMPTY_STRUCT_INIT
#define OPTION_CAST(x) (x)

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// must be included after <windows.h>
#include <shellapi.h>

#include <io.h> // _waccess
#include <fcntl.h> // _O_U8TEXT
#include <direct.h> // _wgetcwd
//#include <WinSock2.h>
#ifdef _MSC_VER
// On MSVC these are the same (as long as /volatile:ms is passed)
#define _Atomic volatile

// MSVC can't parse some things properly
#undef STRUCT_DEFAULT_VALUE
#define STRUCT_DEFAULT_VALUE {0}
#undef EMPTY_STRUCT_DECLARATION
#define EMPTY_STRUCT_DECLARATION void *____dummy_variable;
#undef EMPTY_STRUCT_INIT
#define EMPTY_STRUCT_INIT 0
#undef OPTION_CAST
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

typedef unsigned char byte;
typedef unsigned int uint;
typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
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
typedef array array_uint;
typedef array array_float;
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
#define ALLOC_INIT(type, ...) (type *)memdup((type[]){ __VA_ARGS__ }, sizeof(type))

//================================== GLOBALS =================================*/
//int V_ZERO = 0;
byteptr g_str_buf;
int load_so(byteptr);
void reload_so();
void init_consts();


 int g_test_ok = 1; 
 /*================================== FNS =================================*/
 this line will be replaced with definitions
 /* returns 0 */ array new_array(int mylen, int cap, int elm_size) {
 
array arr= (array) { .len =  mylen , .cap =  cap , .element_size =  elm_size , .data =  v_malloc ( cap * elm_size ) } ;
 
return  arr ;
 
 
 }
 /* returns 0 */ array _make(int len, int cap, int elm_size) {
 
return  new_array ( len , cap , elm_size ) ;
 
 
 }
 /* returns 0 */ array new_array_from_c_array(int len, int cap, int elm_size, void* c_array) {
 
array arr= (array) { .len =  len , .cap =  cap , .element_size =  elm_size , .data =  v_malloc ( cap * elm_size ) } ;
 
 memcpy ( arr .data ,  c_array ,  len * elm_size ) ;
 
return  arr ;
 
 
 }
 /* returns 0 */ array new_array_from_c_array_no_alloc(int len, int cap, int elm_size, void* c_array) {
 
array arr= (array) { .len =  len , .cap =  cap , .element_size =  elm_size , .data =  c_array } ;
 
return  arr ;
 
 
 }
 /* returns 0 */ array array_repeat(void* val, int nr_repeats, int elm_size) {
 
array arr= (array) { .len =  nr_repeats , .cap =  nr_repeats , .element_size =  elm_size , .data =  v_malloc ( nr_repeats * elm_size ) } ;
 
 for (
int i= 0  ;  i < nr_repeats  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * elm_size ,  val ,  elm_size ) ;
 
 }
 ;
 
return  arr ;
 
 
 }
 /* returns 0 */ void array_sort_with_compare(array* a, void* compare) {
 
 qsort ( a ->data ,  a ->len ,  a ->element_size ,  compare ) ;
 
 
 }
 /* returns 0 */ void array_insert(array* a, int i, void* val) {
 
 if ( i >= a ->len ) {
 
 v_panic ( tos2((byte*)"array.insert: index larger than length") ) ;
 
 }
 ;
 
 array__push( a , val ) ;
 
int size= a ->element_size ;
 
 memmove ((byte*) a ->data + ( i + 1 ) * size , (byte*) a ->data + i * size ,  ( a ->len - i ) * size ) ;
 
 array_set( a , i , val ) ;
 
 
 }
 /* returns 0 */ void array_prepend(array* a, void* val) {
 
 array_insert( a , 0 , val ) ;
 
 
 }
 /* returns 0 */ void array_delete(array* a, int idx) {
 
int size= a ->element_size ;
 
 memmove ((byte*) a ->data + idx * size , (byte*) a ->data + ( idx + 1 ) * size ,  ( a ->len - idx ) * size ) ;
 
 a ->len -- ;
 
 a ->cap -- ;
 
 
 }
 /* returns 0 */ void* array__get(array a, int i) {
 
 if ( i < 0  ||  i >= a .len ) {
 
 v_panic ( _STR("array index out of range: %d/%d", i, a .len) ) ;
 
 }
 ;
 
return (byte*) a .data + i * a .element_size ;
 
 
 }
 /* returns 0 */ void* array_first(array a) {
 
 if ( a .len == 0 ) {
 
 v_panic ( tos2((byte*)"array.first: empty array") ) ;
 
 }
 ;
 
return (byte*) a .data + 0 ;
 
 
 }
 /* returns 0 */ void* array_last(array a) {
 
 if ( a .len == 0 ) {
 
 v_panic ( tos2((byte*)"array.last: empty array") ) ;
 
 }
 ;
 
return (byte*) a .data + ( a .len - 1 ) * a .element_size ;
 
 
 }
 /* returns 0 */ array array_left(array s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  array_slice( s , 0 , n ) ;
 
 
 }
 /* returns 0 */ array array_right(array s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  array_slice( s , n , s .len ) ;
 
 
 }
 /* returns 0 */ array array_slice(array s, int start, int _end) {
 
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
 /* returns 0 */ void array_set(array* a, int idx, void* val) {
 
 if ( idx < 0  ||  idx >= a ->len ) {
 
 v_panic ( _STR("array index out of range: %d / %d", idx, a ->len) ) ;
 
 }
 ;
 
 memcpy ((byte*) a ->data + a ->element_size * idx ,  val ,  a ->element_size ) ;
 
 
 }
 /* returns 0 */ void array__push(array* arr, void* val) {
 
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
 /* returns 0 */ void array__push_many(array* arr, void* val, int size) {
 
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
 /* returns 0 */ array array_reverse(array a) {
 
array arr= (array) { .len =  a .len , .cap =  a .cap , .element_size =  a .element_size , .data =  v_malloc ( a .cap * a .element_size ) } ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * arr .element_size ,  & /*vvar*/  ( *(void**) array__get( a , a .len - 1 - i) ) ,  arr .element_size ) ;
 
 }
 ;
 
return  arr ;
 
 
 }
 /* returns 0 */ array array_clone(array a) {
 
array arr= (array) { .len =  a .len , .cap =  a .cap , .element_size =  a .element_size , .data =  v_malloc ( a .cap * a .element_size ) } ;
 
 memcpy ( arr .data ,  a .data ,  a .cap * a .element_size ) ;
 
return  arr ;
 
 
 }
 /* returns 0 */ void v_array_free(array a) {
 
 free ( a .data ) ;
 
 
 }
 /* returns 0 */ string array_string_str(array_string a) {
 
strings__Builder sb= strings__new_builder ( a .len * 3 ) ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"[") ) ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
string val= ( *(string*) array__get( a , i) ) ;
 
 strings__Builder_write(& /* ? */ sb , _STR("\"%.*s\"", val.len, val.str) ) ;
 
 if ( i < a .len - 1 ) {
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)", ") ) ;
 
 }
 ;
 
 }
 ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"]") ) ;
 
return  strings__Builder_str( sb ) ;
 
 
 }
 /* returns 0 */ string array_byte_hex(array_byte b) {
 
byte* hex= v_malloc ( b .len * 2 + 1 ) ;
 
byte* ptr= & /*vvar*/  hex [/*ptr*/ 0 ]/*rbyte 1*/ ;
 
 for (
int i= 0  ;  i < b .len  ;  i ++ ) { 
 
 
 ptr  +=  sprintf ( ptr ,  "%02x" ,  ( *(byte*) array__get( b , i) ) ) ;
 
 }
 ;
 
return  (tos2( hex ) ) ;
 
 
 }
 /* returns 0 */ int copy(array_byte dst, array_byte src) {
 
 if ( dst .len > 0  &&  src .len > 0 ) {
 
int min= ( dst .len < src .len ) ? ( dst .len ) : ( src .len ) ;
 
 memcpy ( dst .data ,  array_left( src , min ) .data ,  dst .element_size * min ) ;
 
return  min ;
 
 }
 ;
 
return  0 ;
 
 
 }
 /* returns 0 */ /* returns 0 */ void todo() {
 
 
 }
 /* returns 0 */ string tos(byte* s, int len) {
 
 if ( isnil ( s ) ) {
 
 v_panic ( tos2((byte*)"tos(): nil string") ) ;
 
 }
 ;
 
return  (string) { .str =  s , .len =  len } ;
 
 
 }
 /* returns 0 */ string tos_clone(byte* s) {
 
 if ( isnil ( s ) ) {
 
 v_panic ( tos2((byte*)"tos: nil string") ) ;
 
 }
 ;
 
return  string_clone( tos2 ( s ) ) ;
 
 
 }
 /* returns 0 */ string tos2(byte* s) {
 
 if ( isnil ( s ) ) {
 
 v_panic ( tos2((byte*)"tos2: nil string") ) ;
 
 }
 ;
 
int len= strlen ( s ) ;
 
string res= tos ( s , len ) ;
 
return  res ;
 
 
 }
 /* returns 0 */ string string_clone(string a) {
 
string b= (string) { .len =  a .len , .str =  v_malloc ( a .len + 1 ) } ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
 b .str[ i ]/*rbyte 1*/  =  a .str[ i ]/*rbyte 0*/ ;
 
 }
 ;
 
 b .str[ a .len ]/*rbyte 1*/  =  '\0' ;
 
return  b ;
 
 
 }
 /* returns 0 */ string string_replace(string s, string rep, string with) {
 
 if ( s .len == 0  ||  rep .len == 0 ) {
 
return  s ;
 
 }
 ;
 
array_int idxs=new_array_from_c_array(0, 0, sizeof(int), (int[]) {EMPTY_STRUCT_INIT   }) ;
 
 {
 
 }
 
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
 /* returns 0 */ int string_int(string s) {
 
return  atoi ( s .str ) ;
 
 
 }
 /* returns 0 */ i32 string_i32(string s) {
 
return  atol ( s .str ) ;
 
 
 }
 /* returns 0 */ i64 string_i64(string s) {
 
return  atoll ( s .str ) ;
 
 
 }
 /* returns 0 */ f32 string_f32(string s) {
 
return  atof ( s .str ) ;
 
 
 }
 /* returns 0 */ f64 string_f64(string s) {
 
return  atof ( s .str ) ;
 
 
 }
 /* returns 0 */ u32 string_u32(string s) {
 
return  strtoul ( s .str ,  0 ,  0 ) ;
 
 
 }
 /* returns 0 */ u64 string_u64(string s) {
 
return  strtoull ( s .str ,  0 ,  0 ) ;
 
 
 }
 /* returns 0 */ bool string_eq(string s, string a) {
 
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
 /* returns 0 */ bool string_ne(string s, string a) {
 
return  ! string_eq( s , a ) ;
 
 
 }
 /* returns 0 */ bool string_lt(string s, string a) {
 
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
 /* returns 0 */ bool string_le(string s, string a) {
 
return  string_lt( s , a )  ||  string_eq( s , a ) ;
 
 
 }
 /* returns 0 */ bool string_gt(string s, string a) {
 
return  ! string_le( s , a ) ;
 
 
 }
 /* returns 0 */ bool string_ge(string s, string a) {
 
return  ! string_lt( s , a ) ;
 
 
 }
 /* returns 0 */ string string_add(string s, string a) {
 
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
 /* returns 0 */ array_string string_split(string s, string delim) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
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
 /* returns 0 */ array_string string_split_single(string s, byte delim) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
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
 /* returns 0 */ array_string string_split_into_lines(string s) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
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
 /* returns 0 */ string string_left(string s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  string_substr( s , 0 , n ) ;
 
 
 }
 /* returns 0 */ string string_right(string s, int n) {
 
 if ( n >= s .len ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_substr( s , n , s .len ) ;
 
 
 }
 /* returns 0 */ string string_substr(string s, int start, int end) {
 
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
 /* returns 0 */ int string_index(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
  
 int tmp53 =  0; 

array_int prefix= array_repeat(&tmp53,  p .len , sizeof(int) ) ;
 
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
  
 int tmp59 =  j; 

array_set(&/*q*/ prefix , i , & tmp59) ;
 
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
 
 v_array_free( prefix ) ;
 
return  i - p .len + 1 ;
 
 }
 ;
 
 }
 ;
 
 v_array_free( prefix ) ;
 
return  - 1 ;
 
 
 }
 /* returns 0 */ int string_index_any(string s, string chars) {
 
 string tmp63 =  chars;
 ;
for (int tmp64 = 0; tmp64 < tmp63 .len; tmp64 ++) {
 byte c = ((byte *) tmp63.str)[tmp64];
 
 
int index= string_index( s , byte_str( c ) ) ;
 
 if ( index != - 1 ) {
 
return  index ;
 
 }
 ;
 
 }
 ;
 
return  - 1 ;
 
 
 }
 /* returns 0 */ int string_last_index(string s, string p) {
 
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
 /* returns 0 */ int string_index_after(string s, string p, int start) {
 
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
 /* returns 0 */ int string_count(string s, string substr) {
 
 if ( s .len == 0  ||  substr .len == 0 ) {
 
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
 /* returns 0 */ bool string_contains(string s, string p) {
 
bool res= string_index( s , p ) > 0 - 1 ;
 
return  res ;
 
 
 }
 /* returns 0 */ bool string_starts_with(string s, string p) {
 
bool res= string_index( s , p ) == 0 ;
 
return  res ;
 
 
 }
 /* returns 0 */ bool string_ends_with(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  0 ;
 
 }
 ;
 
bool res= string_last_index( s , p ) == s .len - p .len ;
 
return  res ;
 
 
 }
 /* returns 0 */ string string_to_lower(string s) {
 
byte* b= v_malloc ( s .len ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 b [/*ptr*/ i ]/*rbyte 1*/  =  tolower ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
 }
 ;
 
return  tos ( b , s .len ) ;
 
 
 }
 /* returns 0 */ string string_to_upper(string s) {
 
byte* b= v_malloc ( s .len ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 b [/*ptr*/ i ]/*rbyte 1*/  =  toupper ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
 }
 ;
 
return  tos ( b , s .len ) ;
 
 
 }
 /* returns 0 */ string string_find_between(string s, string start, string end) {
 
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
 /* returns 0 */ bool array_string_contains(array_string ar, string val) {
 
 array_string tmp84 =  ar;
 ;
for (int tmp85 = 0; tmp85 < tmp84 .len; tmp85 ++) {
 string s = ((string *) tmp84.data)[tmp85];
 
 
 if (string_eq( s , val ) ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 
 }
 /* returns 0 */ bool array_int_contains(array_int ar, int val) {
 
 array_int tmp86 =  ar ;
 ;
for (int i = 0; i < tmp86 .len; i ++) {
 int s = ((int *) tmp86 . data)[i];
 
 
 if ( s == val ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 
 }
 /* returns 0 */ bool is_space(byte c) {
 
return  isspace ( c ) ;
 
 
 }
 /* returns 0 */ bool byte_is_space(byte c) {
 
return  is_space ( c ) ;
 
 
 }
 /* returns 0 */ string string_trim_space(string s) {
 
 if (string_eq( s , tos2((byte*)"") ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
int i= 0 ;
 
 while ( i < s .len  &&  is_space ( s .str[ i ]/*rbyte 0*/ ) ) {
 
 
 i ++ ;
 
 }
 ;
 
int end= s .len - 1 ;
 
 while ( end >= 0  &&  is_space ( s .str[ end ]/*rbyte 0*/ ) ) {
 
 
 end -- ;
 
 }
 ;
 
 if ( i > end + 1 ) {
 
return  s ;
 
 }
 ;
 
string res= string_substr( s , i , end + 1 ) ;
 
return  res ;
 
 
 }
 /* returns 0 */ string string_trim(string s, byte c) {
 
 if (string_eq( s , tos2((byte*)"") ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
int i= 0 ;
 
 while ( i < s .len  &&  c == s .str[ i ]/*rbyte 0*/ ) {
 
 
 i ++ ;
 
 }
 ;
 
string res= string_right( s , i ) ;
 
int end= res .len - 1 ;
 
 while ( end >= 0  &&  c == res .str[ end ]/*rbyte 1*/ ) {
 
 
 end -- ;
 
 }
 ;
 
 res  =  string_left( res , end + 1 ) ;
 
return  res ;
 
 
 }
 /* returns 0 */ string string_trim_left(string s, string cutset) {
 
int start= string_index( s , cutset ) ;
 
 if ( start != 0 ) {
 
return  s ;
 
 }
 ;
 
 while ( start < s .len - 1  &&  s .str[ start ]/*rbyte 0*/ == cutset .str[ 0 ]/*rbyte 0*/ ) {
 
 
 start ++ ;
 
 }
 ;
 
return  string_right( s , start ) ;
 
 
 }
 /* returns 0 */ string string_trim_right(string s, string cutset) {
 
 if ( s .len == 0 ) {
 
return  s ;
 
 }
 ;
 
int pos= s .len - 1 ;
 
 while ( s .str[ pos ]/*rbyte 0*/ == cutset .str[ 0 ]/*rbyte 0*/ ) {
 
 
 pos -- ;
 
 }
 ;
 
return  string_left( s , pos + 1 ) ;
 
 
 }
 /* returns 0 */ int compare_strings(string* a, string* b) {
 
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
 /* returns 0 */ int compare_strings_by_len(string* a, string* b) {
 
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
 /* returns 0 */ int compare_lower_strings(string* a, string* b) {
 
string aa= string_to_lower(* a ) ;
 
string bb= string_to_lower(* b ) ;
 
return  compare_strings (& /*112 EXP:"string*" GOT:"string" */ aa ,& /*112 EXP:"string*" GOT:"string" */ bb ) ;
 
 
 }
 /* returns 0 */ void array_string_sort(array_string* s) {
 
 array_sort_with_compare( s , compare_strings ) ;
 
 
 }
 /* returns 0 */ void array_string_sort_ignore_case(array_string* s) {
 
 array_sort_with_compare( s , compare_lower_strings ) ;
 
 
 }
 /* returns 0 */ void array_string_sort_by_len(array_string* s) {
 
 array_sort_with_compare( s , compare_strings_by_len ) ;
 
 
 }
 /* returns 0 */ ustring string_ustring(string s) {
 
ustring res= (ustring) { .s =  s , .runes =  new_array ( 0 , s .len , sizeof( int) ) , .len = 0 } ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
int char_len= utf8_char_len ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
_PUSH(& res .runes , ( i ), tmp100, int) ;
 
 i  +=  char_len - 1 ;
 
 res .len ++ ;
 
 }
 ;
 
return  res ;
 
 
 }
 /* returns 0 */ ustring string_ustring_tmp(string s) {
 
 if ( g_ustring_runes .len == 0 ) {
 
 g_ustring_runes  =  new_array ( 0 , 128 , sizeof( int) ) ;
 
 }
 ;
 
ustring res= (ustring) { .s =  s , .runes = new_array(0, 1, sizeof( int )) , .len = 0 } ;
 
 res .runes  =  g_ustring_runes ;
 
 res .runes .len  =  s .len ;
 
int j= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
int char_len= utf8_char_len ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
  
 int tmp105 =  i; 

array_set(&/*q*/ res .runes , j , & tmp105) ;
 
 j ++ ;
 
 i  +=  char_len - 1 ;
 
 res .len ++ ;
 
 }
 ;
 
return  res ;
 
 
 }
 /* returns 0 */ string ustring_substr(ustring u, int _start, int _end) {
 
int start= ( *(int*) array__get( u .runes , _start) ) ;
 
int end= ( _end >= u .runes .len ) ? ( u .s .len ) : ( ( *(int*) array__get( u .runes , _end) ) ) ;
 
return  string_substr( u .s , start , end ) ;
 
 
 }
 /* returns 0 */ string ustring_left(ustring u, int pos) {
 
return  ustring_substr( u , 0 , pos ) ;
 
 
 }
 /* returns 0 */ string ustring_right(ustring u, int pos) {
 
return  ustring_substr( u , pos , u .len ) ;
 
 
 }
 /* returns 0 */ byte string_at(string s, int idx) {
 
 if ( idx < 0  ||  idx >= s .len ) {
 
 v_panic ( _STR("string index out of range: %d / %d", idx, s .len) ) ;
 
 }
 ;
 
return  s .str [/*ptr*/ idx ]/*rbyte 0*/ ;
 
 
 }
 /* returns 0 */ string ustring_at(ustring u, int idx) {
 
return  ustring_substr( u , idx , idx + 1 ) ;
 
 
 }
 /* returns 0 */ void v_ustring_free(ustring u) {
 
 v_array_free( u .runes ) ;
 
 
 }
 /* returns 0 */ bool byte_is_digit(byte c) {
 
return  c >= '0'  &&  c <= '9' ;
 
 
 }
 /* returns 0 */ bool byte_is_hex_digit(byte c) {
 
return  byte_is_digit( c )  ||  ( c >= 'a'  &&  c <= 'f' )  ||  ( c >= 'A'  &&  c <= 'F' ) ;
 
 
 }
 /* returns 0 */ bool byte_is_oct_digit(byte c) {
 
return  c >= '0'  &&  c <= '7' ;
 
 
 }
 /* returns 0 */ bool byte_is_letter(byte c) {
 
return  ( c >= 'a'  &&  c <= 'z' )  ||  ( c >= 'A'  &&  c <= 'Z' ) ;
 
 
 }
 /* returns 0 */ void v_string_free(string s) {
 
 v_free ( s .str ) ;
 
 
 }
 /* returns 0 */ string string_all_before(string s, string dot) {
 
int pos= string_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_left( s , pos ) ;
 
 
 }
 /* returns 0 */ string string_all_before_last(string s, string dot) {
 
int pos= string_last_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_left( s , pos ) ;
 
 
 }
 /* returns 0 */ string string_all_after(string s, string dot) {
 
int pos= string_last_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_right( s , pos + dot .len ) ;
 
 
 }
 /* returns 0 */ string array_string_join(array_string a, string del) {
 
 if ( a .len == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
int len= 0 ;
 
 array_string tmp116 =  a ;
 ;
for (int i = 0; i < tmp116 .len; i ++) {
 string val = ((string *) tmp116 . data)[i];
 
 
 len  +=  val .len + del .len ;
 
 }
 ;
 
 len  -=  del .len ;
 
string res= tos2((byte*)"") ;
 
 res .len  =  len ;
 
 res .str  =  v_malloc ( res .len + 1 ) ;
 
int idx= 0 ;
 
 array_string tmp119 =  a ;
 ;
for (int i = 0; i < tmp119 .len; i ++) {
 string val = ((string *) tmp119 . data)[i];
 
 
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
 /* returns 0 */ string array_string_join_lines(array_string s) {
 
return  array_string_join( s , tos2((byte*)"\n") ) ;
 
 
 }
 /* returns 0 */ string string_reverse(string s) {
 
string res= (string) { .len =  s .len , .str =  v_malloc ( s .len ) } ;
 
 for (
int i= s .len - 1  ;  i >= 0  ;  i -- ) { 
 
 
 res .str[ s .len - i - 1 ]/*rbyte 1*/  =  s .str[ i ]/*rbyte 0*/ ;
 
 }
 ;
 
return  res ;
 
 
 }
 /* returns 0 */ string string_limit(string s, int max) {
 
ustring u= string_ustring( s ) ;
 
 if ( u .len <= max ) {
 
return  s ;
 
 }
 ;
 
return  ustring_substr( u , 0 , max ) ;
 
 
 }
 /* returns 0 */ bool byte_is_white(byte c) {
 
int i= ((int)( c ) ) ;
 
return  i == 10  ||  i == 32  ||  i == 9  ||  i == 13  ||  c == '\r' ;
 
 
 }
 /* returns 0 */ int string_hash(string s) {
 
int h= 0 ;
 
 if ( h == 0  &&  s .len > 0 ) {
 
 string tmp128 =  s;
 ;
for (int tmp129 = 0; tmp129 < tmp128 .len; tmp129 ++) {
 byte c = ((byte *) tmp128.str)[tmp129];
 
 
 h  =  h * 31 + ((int)( c ) ) ;
 
 }
 ;
 
 }
 ;
 
return  h ;
 
 
 }
 /* returns 0 */ array_byte string_bytes(string s) {
 
 if ( s .len == 0 ) {
 
return new_array_from_c_array(0, 0, sizeof(byte), (byte[]) {EMPTY_STRUCT_INIT   }) ;
 
 }
 ;
  
 byte tmp130 =  ((byte)( 0 ) ); 

array_byte buf= array_repeat(&tmp130,  s .len , sizeof(byte) ) ;
 
 memcpy ( buf .data ,  s .str ,  s .len ) ;
 
return  buf ;
 
 
 }
 /* returns 0 */ void v_exit(int code) {
 
 exit ( code ) ;
 
 
 }
 /* returns 0 */ bool isnil(void* v) {
 
return  v == 0 ;
 
 
 }
 /* returns 0 */ void on_panic(int (*f)( int  /*FFF*/ )) {
 
 
 }
 /* returns 0 */ void print_backtrace() {
 
 if ( 1 ) {
 
 return ;
 
 }
 ;
 
 #ifdef __APPLE__
 
voidptr buffer  [100 ]= STRUCT_DEFAULT_VALUE ;
 
void* nr_ptrs= backtrace ( buffer ,  100 ) ;
 
 backtrace_symbols_fd ( buffer ,  nr_ptrs ,  1 ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ void _panic_debug(int line_no, string file, string mod, string fn_name, string s) {
 
 println ( tos2((byte*)"================ V panic ================") ) ;
 
printf( "   module: %.*s\n", mod.len, mod.str ) ;
 
printf( " function: %.*s()\n", fn_name.len, fn_name.str ) ;
 
printf( "     file: %.*s\n", file.len, file.str ) ;
 
 println (string_add( tos2((byte*)"     line: ") , int_str( line_no ) ) ) ;
 
printf( "  message: %.*s\n", s.len, s.str ) ;
 
 println ( tos2((byte*)"=========================================") ) ;
 
 print_backtrace ( ) ;
 
 exit ( 1 ) ;
 
 
 }
 /* returns 0 */ void v_panic(string s) {
 
printf( "V panic: %.*s\n", s.len, s.str ) ;
 
 print_backtrace ( ) ;
 
 exit ( 1 ) ;
 
 
 }
 /* returns 0 */ void println(string s) {
 
 if ( isnil ( s .str ) ) {
 
 v_panic ( tos2((byte*)"println(NIL)") ) ;
 
 }
 ;
 
 #ifdef _WIN32
 
 _putws ( string_to_wide( s ) ) ;
 
 /* returns 0 */ #else
 
 printf ( "%.*s\n" ,  s .len ,  s .str ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ void eprintln(string s) {
 
 if ( isnil ( s .str ) ) {
 
 v_panic ( tos2((byte*)"eprintln(NIL)") ) ;
 
 }
 ;
 
 #ifdef __APPLE__
 
 fprintf ( stderr ,  "%.*s\n" ,  s .len ,  s .str ) ;
 
 /* returns 0 */ #else
 
 println ( s ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ void v_print(string s) {
 
 #ifdef _WIN32
 
 wprintf ( string_to_wide( s ) ) ;
 
 /* returns 0 */ #else
 
 printf ( "%.*s" ,  s .len ,  s .str ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ byte* v_malloc(int n) {
 
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
 /* returns 0 */ byte* v_calloc(int n) {
 
 if ( n < 0 ) {
 
 v_panic ( tos2((byte*)"calloc(<0)") ) ;
 
 }
 ;
 
return  calloc ( n ,  1 ) ;
 
 
 }
 /* returns 0 */ void v_free(void* ptr) {
 
 free ( ptr ) ;
 
 
 }
 /* returns 0 */ void* memdup(void* src, int sz) {
 
byte* mem= v_malloc ( sz ) ;
 
return  memcpy ( mem ,  src ,  sz ) ;
 
 
 }
 /* returns 0 */ string double_str(double d) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( buf ,  "%f" ,  d ) ;
 
return  tos ( buf , strlen ( buf ) ) ;
 
 
 }
 /* returns 0 */ string f64_str(f64 d) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( buf ,  "%f" ,  d ) ;
 
return  tos ( buf , strlen ( buf ) ) ;
 
 
 }
 /* returns 0 */ string f32_str(f32 d) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( buf ,  "%f" ,  d ) ;
 
return  tos ( buf , strlen ( buf ) ) ;
 
 
 }
 /* returns 0 */ string ptr_str(void* ptr) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( buf ,  "%p" ,  ptr ) ;
 
return  tos ( buf , strlen ( buf ) ) ;
 
 
 }
 /* returns 0 */ bool f64_eq(f64 a, f64 b) {
 
return  fabs ( a - b ) <= DBL_EPSILON ;
 
 
 }
 /* returns 0 */ string int_str(int nn) {
 
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
 /* returns 0 */ string u32_str(u32 nn) {
 
u32 n= nn ;
 
 if ( n == ((u32)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 16 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
bool is_neg= 0 ;
 
 if ( n < ((u32)( 0 ) ) ) {
 
 n  =  - n ;
 
 is_neg  =  1 ;
 
 }
 ;
 
 while ( n > ((u32)( 0 ) ) ) {
 
 
u32 d= n % ((u32)( 10 ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u32)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((u32)( 10 ) ) ;
 
 }
 ;
 
 if ( is_neg ) {
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 
 len ++ ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 
 }
 /* returns 0 */ string u8_str(u8 nn) {
 
u8 n= nn ;
 
 if ( n == ((u8)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 5 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
bool is_neg= 0 ;
 
 if ( n < ((u8)( 0 ) ) ) {
 
 n  =  - n ;
 
 is_neg  =  1 ;
 
 }
 ;
 
 while ( n > ((u8)( 0 ) ) ) {
 
 
u8 d= n % ((u8)( 10 ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u8)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((u8)( 10 ) ) ;
 
 }
 ;
 
 if ( is_neg ) {
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 
 len ++ ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 
 }
 /* returns 0 */ string i64_str(i64 nn) {
 
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
 /* returns 0 */ string u64_str(u64 nn) {
 
u64 n= nn ;
 
 if ( n == ((u64)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 32 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
bool is_neg= 0 ;
 
 if ( n < ((u64)( 0 ) ) ) {
 
 n  =  - n ;
 
 is_neg  =  1 ;
 
 }
 ;
 
 while ( n > ((u64)( 0 ) ) ) {
 
 
u64 d= n % ((u64)( 10 ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u64)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((u64)( 10 ) ) ;
 
 }
 ;
 
 if ( is_neg ) {
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 
 len ++ ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 
 }
 /* returns 0 */ string bool_str(bool b) {
 
 if ( b ) {
 
return  tos2((byte*)"true") ;
 
 }
 ;
 
return  tos2((byte*)"false") ;
 
 
 }
 /* returns 0 */ string int_hex(int n) {
 
int len= ( n >= 0 ) ? ( int_str( n ) .len + 3 ) : ( 11 ) ;
 
byte* hex= v_malloc ( len ) ;
 
int count= ((int)( sprintf ( hex ,  "0x%x" ,  n ) ) ) ;
 
return  tos ( hex , count ) ;
 
 
 }
 /* returns 0 */ string i64_hex(i64 n) {
 
int len= ( n >= ((i64)( 0 ) ) ) ? ( i64_str( n ) .len + 3 ) : ( 19 ) ;
 
byte* hex= v_malloc ( len ) ;
 
int count= ((int)( sprintf ( hex ,  "0x%x" ,  n ) ) ) ;
 
return  tos ( hex , count ) ;
 
 
 }
 /* returns 0 */ bool array_byte_contains(array_byte a, byte val) {
 
 array_byte tmp41 =  a;
 ;
for (int tmp42 = 0; tmp42 < tmp41 .len; tmp42 ++) {
 byte aa = ((byte *) tmp41.data)[tmp42];
 
 
 if ( aa == val ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 
 }
 /* returns 0 */ string rune_str(rune c) {
 
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
 /* returns 0 */ string byte_str(byte c) {
 
string str= (string) { .len =  1 , .str =  v_malloc ( 2 ) } ;
 
 str .str [/*ptr*/ 0 ]/*rbyte 1*/  =  c ;
 
 str .str [/*ptr*/ 1 ]/*rbyte 1*/  =  '\0' ;
 
return  str ;
 
 
 }
 /* returns 0 */ bool byte_is_capital(byte c) {
 
return  c >= 'A'  &&  c <= 'Z' ;
 
 
 }
 /* returns 0 */ array_byte array_byte_clone(array_byte b) {
  
 byte tmp48 =  ((byte)( 0 ) ); 

array_byte res= array_repeat(&tmp48,  b .len , sizeof(byte) ) ;
 
 for (
int i= 0  ;  i < b .len  ;  i ++ ) { 
 
  
 byte tmp53 =  ( *(byte*) array__get( b , i) ); 

array_set(&/*q*/ res , i , & tmp53) ;
 
 }
 ;
 
return  res ;
 
 
 }
 /* returns 0 */ int utf8_char_len(byte b) {
 
return  ( ( 0xe5000000  >>  ( ( b  >>  3 ) & 0x1e ) ) & 3 ) + 1 ;
 
 
 }
 /* returns 0 */ string utf32_to_str(u32 code) {
 
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
 /* returns 0 */ string utf32_to_str_no_malloc(u32 code, void* buf) {
 
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
 /* returns 0 */ int string_utf32_code(string _rune) {
 
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
 /* returns 0 */ u16* string_to_wide(string _str) {
 
 #ifdef _WIN32
 
int num_chars= ((int)( MultiByteToWideChar ( builtin__CP_UTF8 ,  0 ,  _str .str ,  _str .len ,  0 ,  0 ) ) ) ;
 
u16* wstr= ((u16*)( v_malloc ( ( num_chars + 1 ) * 2 ) ) ) ;
 
 if ( wstr > 0 ) {
 
 MultiByteToWideChar ( builtin__CP_UTF8 ,  0 ,  _str .str ,  _str .len ,  wstr ,  num_chars ) ;
 
 memset ( ((byte*)( wstr ) ) + num_chars * 2 ,  0 ,  2 ) ;
 
 }
 ;
 
return  wstr ;
 
 /* returns 0 */ #else
 
return  0 ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ string string_from_wide(u16* _wstr) {
 
 #ifdef _WIN32
 
int wstr_len= ((int)( wcslen ( _wstr ) ) ) ;
 
return  string_from_wide2 ( _wstr , wstr_len ) ;
 
 /* returns 0 */ #else
 
return  tos2((byte*)"") ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ string string_from_wide2(u16* _wstr, int len) {
 
 #ifdef _WIN32
 
int num_chars= ((int)( WideCharToMultiByte ( builtin__CP_UTF8 ,  0 ,  _wstr ,  len ,  0 ,  0 ,  0 ,  0 ) ) ) ;
 
byte* str_to= ((byte*)( v_malloc ( num_chars + 1 ) ) ) ;
 
 if ( str_to > 0 ) {
 
 WideCharToMultiByte ( builtin__CP_UTF8 ,  0 ,  _wstr ,  len ,  str_to ,  num_chars ,  0 ,  0 ) ;
 
 memset ( ((byte*)( str_to ) ) + num_chars ,  0 ,  1 ) ;
 
 }
 ;
 
return  tos2 ( str_to ) ;
 
 /* returns 0 */ #else
 
return  tos2((byte*)"") ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ map new_map(int cap, int elm_size) {
 
map res= (map) { .element_size =  elm_size , .root =  0 , .size = 0 } ;
 
return  res ;
 
 
 }
 /* returns 0 */ map new_map_init(int cap, int elm_size, string* keys, void* vals) {
 
map res= (map) { .element_size =  elm_size , .root =  0 , .size = 0 } ;
 
 int tmp3 =  0;
 ;
for (int tmp4 = tmp3; tmp4 <  cap; tmp4++) {
 int i = tmp4;
 
 
 map__set(& /* ? */ res , keys [/*ptr*/ i ]/*rstring 0*/ ,(byte*) vals + i * elm_size ) ;
 
 }
 ;
 
return  res ;
 
 
 }
 /* returns 0 */ Node* new_node(string key, void* val, int element_size) {
 
Node* new_e= ALLOC_INIT(Node, { .key =  key , .val =  v_malloc ( element_size ) , .left =  0 , .right =  0 , .is_empty = 0 , } ) ;
 
 memcpy ( new_e ->val ,  val ,  element_size ) ;
 
return  new_e ;
 
 
 }
 /* returns 0 */ void map_insert(map* m, Node* n, string key, void* val) {
 
 if (string_eq( n ->key , key ) ) {
 
 memcpy ( n ->val ,  val ,  m ->element_size ) ;
 
 return ;
 
 }
 ;
 
 if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
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
 
 if ( isnil ( n ->right ) ) {
 
 n ->right  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 }
  else { 
 
 map_insert( m , n ->right , key , val ) ;
 
 }
 ;
 
 
 }
 /* returns 0 */ bool Node_find(Node* n, string key, void* out, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
 memcpy ( out ,  n ->val ,  element_size ) ;
 
return  1 ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
return  0 ;
 
 }
  else { 
 
return  Node_find(& /* ? */* n ->left , key , out , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( isnil ( n ->right ) ) {
 
return  0 ;
 
 }
  else { 
 
return  Node_find(& /* ? */* n ->right , key , out , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 
 }
 /* returns 0 */ bool Node_find2(Node* n, string key, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
return  1 ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
return  0 ;
 
 }
  else { 
 
return  Node_find2(& /* ? */* n ->left , key , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( isnil ( n ->right ) ) {
 
return  0 ;
 
 }
  else { 
 
return  Node_find2(& /* ? */* n ->right , key , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 
 }
 /* returns 0 */ void map__set(map* m, string key, void* val) {
 
 if ( isnil ( m ->root ) ) {
 
 m ->root  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 return ;
 
 }
 ;
 
 map_insert( m , m ->root , key , val ) ;
 
 
 }
 /* returns 0 */ int preorder_keys(Node* node, array_string* keys, int key_i) {
 
int i= key_i ;
 
 if ( ! node ->is_empty ) {
 
array_string a= * keys ;
  
 string tmp8 =  node ->key; 

array_set(&/*q*/ a , i , & tmp8) ;
 
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
 /* returns 0 */ array_string map_keys(map* m) {
  
 string tmp9 =  tos2((byte*)""); 

array_string keys= array_repeat(&tmp9,  m ->size , sizeof(string) ) ;
 
 if ( isnil ( m ->root ) ) {
 
return  keys ;
 
 }
 ;
 
 preorder_keys ( m ->root ,& /*111*/ (array[]){ keys }  , 0 ) ;
 
return  keys ;
 
 
 }
 /* returns 0 */ bool map_get(map m, string key, void* out) {
 
 if ( isnil ( m .root ) ) {
 
return  0 ;
 
 }
 ;
 
return  Node_find(& /* ? */* m .root , key , out , m .element_size ) ;
 
 
 }
 /* returns 0 */ void Node_delete(Node* n, string key, int element_size) {
 
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
 
 Node_delete( n ->left , key , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( isnil ( n ->right ) ) {
 
 return ;
 
 }
  else { 
 
 Node_delete( n ->right , key , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 
 }
 /* returns 0 */ void map_delete(map* m, string key) {
 
 Node_delete( m ->root , key , m ->element_size ) ;
 
 m ->size -- ;
 
 
 }
 /* returns 0 */ bool map_exists(map m, string key) {
 
 v_panic ( tos2((byte*)"map.exists(key) was removed from the language. Use `key in map` instead.") ) ;
 
 
 }
 /* returns 0 */ bool map__exists(map m, string key) {
 
return  ! isnil ( m .root )  &&  Node_find2(& /* ? */* m .root , key , m .element_size ) ;
 
 
 }
 /* returns 0 */ void v_map_print(map m) {
 
 println ( tos2((byte*)"<<<<<<<<") ) ;
 
 println ( tos2((byte*)">>>>>>>>>>") ) ;
 
 
 }
 /* returns 0 */ void v_map_free(map m) {
 
 
 }
 /* returns 0 */ string map_string_str(map_string m) {
 
 if ( m .size == 0 ) {
 
return  tos2((byte*)"{}") ;
 
 }
 ;
 
strings__Builder sb= strings__new_builder ( 50 ) ;
 
 strings__Builder_writeln(& /* ? */ sb , tos2((byte*)"{") ) ;
 
 map_string tmp12 =  m ;
 array_string keys_tmp12 = map_keys(& tmp12 ); 
 for (int l = 0; l < keys_tmp12 .len; l++) {
   string key = ((string*)keys_tmp12 .data)[l];
 string val = {0}; map_get(tmp12, key, & val);
 
 
 strings__Builder_writeln(& /* ? */ sb , _STR("  \"%.*s\" => \"%.*s\"", key.len, key.str, val.len, val.str) ) ;
 
 }
 ;
 
 strings__Builder_writeln(& /* ? */ sb , tos2((byte*)"}") ) ;
 
return  strings__Builder_str( sb ) ;
 
 
 }
 /* returns 0 */ Option opt_ok(void* data, int size) {
 
 if ( size >= 255 ) {
 
 v_panic ( _STR("option size too big: %d (max is 255), this is a temporary limit", size) ) ;
 
 }
 ;
 
Option res= (Option) { .ok =  1 , .error = tos("", 0) , } ;
 
 memcpy ( res .data ,  data ,  size ) ;
 
return  res ;
 
 
 }
 /* returns 0 */ Option v_error(string s) {
 
return  (Option) { .error =  s , .ok = 0 } ;
 
 
 }
 /* returns 0 */ strings__Builder strings__new_builder(int initial_size) {
 
return  (strings__Builder) { .buf =  _make ( 0 , initial_size , sizeof( byte) ) , .len = 0 } ;
 
 
 }
 /* returns 0 */ void strings__Builder_write(strings__Builder* b, string s) {
 
 array__push_many(& /* ? */ b ->buf , s .str , s .len ) ;
 
 b ->len  +=  s .len ;
 
 
 }
 /* returns 0 */ void strings__Builder_writeln(strings__Builder* b, string s) {
 
 array__push_many(& /* ? */ b ->buf , s .str , s .len ) ;
 
_PUSH(& b ->buf , ( '\n' ), tmp1, byte) ;
 
 b ->len  +=  s .len + 1 ;
 
 
 }
 /* returns 0 */ string strings__Builder_str(strings__Builder b) {
 
return  (tos( b .buf .data ,  b .len ) ) ;
 
 
 }
 /* returns 0 */ void strings__Builder_cut(strings__Builder* b, int n) {
 
 b ->len  -=  n ;
 
 
 }
 /* returns 0 */ void strings__Builder_free(strings__Builder* b) {
 
 v_free ( b ->buf .data ) ;
 
 
 }
 /* returns 0 */ string strings__repeat(byte c, int n) {
 
 if ( n <= 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
byte* arr= v_malloc ( n + 1 ) ;
 
 for (
int i= 0  ;  i < n  ;  i ++ ) { 
 
 
 arr [/*ptr*/ i ]/*rbyte 1*/  =  c ;
 
 }
 ;
 
 arr [/*ptr*/ n ]/*rbyte 1*/  =  '\0' ;
 
return  (tos( arr ,  n ) ) ;
 
 
 }
 /* returns 0 */ Option_string net__hostname() {
 
byte name  [256 ]= STRUCT_DEFAULT_VALUE ;
 
void* res= gethostname ( & /*vvar*/  name ,  256 ) ;
 
 if ( res != 0 ) {
 
return  v_error ( tos2((byte*)"net.hostname() cannot get the host name") ) ;
 
 }
 ;
 
string tmp3 = OPTION_CAST(string)( tos_clone ( name )); return opt_ok(&tmp3, sizeof(string)) ;
 
 
 }
 /* returns 0 */ Option_net__Socket net__socket(int family, int _type, int proto) {
 
 #ifdef _WIN32
  
 struct /*c struct init*/ 

WSAData wsadata= (struct WSAData) { .wVersion = 0 , .wHighVersion = 0 , .iMaxSockets = 0 , .iMaxUdpDg = 0 , .lpVendorInfo = 0 } ;
 
void* res= WSAStartup ( net__WSA_V22 ,  & /*vvar*/  wsadata ) ;
 
 if ( res != 0 ) {
 
return  v_error ( tos2((byte*)"socket: WSAStartup failed") ) ;
 
 }
 ;
 
 #endif
 /* returns 0 */ ;
 
void* sockfd= socket ( family ,  _type ,  proto ) ;
 
int one= 1 ;
 
 setsockopt ( sockfd ,  SOL_SOCKET ,  SO_REUSEADDR ,  & /*vvar*/  one ,  sizeof( int) ) ;
 
 if ( sockfd == 0 ) {
 
return  v_error ( tos2((byte*)"socket: init failed") ) ;
 
 }
 ;
 
net__Socket s= (net__Socket) { .sockfd =  sockfd , .family =  family , ._type =  _type , .proto =  proto } ;
 
net__Socket tmp6 = OPTION_CAST(net__Socket)( s); return opt_ok(&tmp6, sizeof(net__Socket)) ;
 
 
 }
 /* returns 0 */ Option_net__Socket net__socket_udp() {
 
Option_net__Socket tmp7 = OPTION_CAST(Option_net__Socket)( net__socket ( AF_INET , SOCK_DGRAM , IPPROTO_UDP )); return opt_ok(&tmp7, sizeof(net__Socket)) ;
 
 
 }
 /* returns 0 */ Option_int net__Socket_setsockopt(net__Socket s, int level, int optname, int* optvalue) {
 
void* res= setsockopt ( s .sockfd ,  level ,  optname ,  optvalue ,   ( optvalue ) ) ;
 
 if ( res < 0 ) {
 
return  v_error ( tos2((byte*)"socket: setsockopt failed") ) ;
 
 }
 ;
 
int tmp9 = OPTION_CAST(int)( ((int)( res ) )); return opt_ok(&tmp9, sizeof(int)) ;
 
 
 }
 /* returns 0 */ Option_int net__Socket_bind(net__Socket s, int port) {
  
 struct /*c struct init*/ 

sockaddr_in addr= (struct sockaddr_in) { .sin_family = 0 , .sin_port = 0 , } ;
 
 addr .sin_family  =  s .family ;
 
 addr .sin_port  =  htons ( port ) ;
 
 addr .sin_addr .s_addr  =  htonl ( INADDR_ANY ) ;
 
int size= 16 ;
 
int res= ((int)( bind ( s .sockfd ,  & /*vvar*/  addr ,  size ) ) ) ;
 
 if ( res < 0 ) {
 
return  v_error ( tos2((byte*)"socket: bind failed") ) ;
 
 }
 ;
 
int tmp13 = OPTION_CAST(int)( res); return opt_ok(&tmp13, sizeof(int)) ;
 
 
 }
 /* returns 0 */ Option_int net__Socket_listen(net__Socket s) {
 
int backlog= 128 ;
 
int res= ((int)( listen ( s .sockfd ,  backlog ) ) ) ;
 
 if ( res < 0 ) {
 
return  v_error ( tos2((byte*)"socket: listen failed") ) ;
 
 }
 ;
 
 #ifdef VDEBUG
 
printf( "listen res = %d\n", res ) ;
 
 #endif
 /* returns 0 */ ;
 
int tmp16 = OPTION_CAST(int)( res); return opt_ok(&tmp16, sizeof(int)) ;
 
 
 }
 /* returns 0 */ Option_int net__Socket_listen_backlog(net__Socket s, int backlog) {
 
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
 /* returns 0 */ Option_net__Socket net__listen(int port) {
 
 #ifdef VDEBUG
 
printf( "net.listen(%d)\n", port ) ;
 
 #endif
 /* returns 0 */ ;
 
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
 /* returns 0 */ Option_net__Socket net__Socket_accept(net__Socket s) {
 
 #ifdef VDEBUG
 
 println ( tos2((byte*)"accept()") ) ;
 
 #endif
 /* returns 0 */ ;
  
 struct /*c struct init*/ 

sockaddr_storage addr= (struct sockaddr_storage) { EMPTY_STRUCT_INIT } ;
 
int size= 128 ;
 
void* sockfd= accept ( s .sockfd ,  & /*vvar*/  addr ,  & /*vvar*/  size ) ;
 
 if ( sockfd < 0 ) {
 
return  v_error ( tos2((byte*)"socket: accept failed") ) ;
 
 }
 ;
 
net__Socket c= (net__Socket) { .sockfd =  sockfd , .family =  s .family , ._type =  s ._type , .proto =  s .proto } ;
 
net__Socket tmp28 = OPTION_CAST(net__Socket)( c); return opt_ok(&tmp28, sizeof(net__Socket)) ;
 
 
 }
 /* returns 0 */ Option_int net__Socket_connect(net__Socket s, string address, int port) {
  
 struct /*c struct init*/ 

addrinfo hints= (struct addrinfo) { .ai_family = 0 , .ai_socktype = 0 , .ai_flags = 0 , .ai_protocol = 0 , .ai_addrlen = 0 , .ai_next = 0 , .ai_addr = 0 } ;
 
 hints .ai_family  =  AF_UNSPEC ;
 
 hints .ai_socktype  =  SOCK_STREAM ;
 
 hints .ai_flags  =  AI_PASSIVE ;
  
 struct /*c struct init*/ 

addrinfo* info= 0 ;
 
string sport= _STR("%d", port) ;
 
void* info_res= getaddrinfo ( address .str ,  sport .str ,  & /*vvar*/  hints ,  & /*vvar*/  info ) ;
 
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
 /* returns 0 */ Option_net__Socket net__dial(string address, int port) {
 
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
 /* returns 0 */ int net__Socket_send(net__Socket s, byte* buf, int len) {
 
void* res= send ( s .sockfd ,  buf ,  len ,  0 ) ;
 
return  res ;
 
 
 }
 /* returns 0 */ byte* net__Socket_recv(net__Socket s, int bufsize) {
 
byte* buf= v_malloc ( bufsize ) ;
 
void* res= recv ( s .sockfd ,  buf ,  bufsize ,  0 ) ;
 
return  buf ;
 
 
 }
 /* returns 0 */ int net__Socket_cread(net__Socket s, byte* buffer, int buffersize) {
 
return  ((int)( read ( s .sockfd ,  buffer ,  buffersize ) ) ) ;
 
 
 }
 /* returns 0 */ int net__Socket_crecv(net__Socket s, byte* buffer, int buffersize) {
 
return  ((int)( recv ( s .sockfd ,  buffer ,  buffersize ,  0 ) ) ) ;
 
 
 }
 /* returns 0 */ Option_int net__Socket_close(net__Socket s) {
 
int shutdown_res= 0 ;
 
 #ifdef _WIN32
 
 shutdown_res  =  shutdown ( s .sockfd ,  SD_BOTH ) ;
 
 /* returns 0 */ #else
 
 shutdown_res  =  shutdown ( s .sockfd ,  SHUT_RDWR ) ;
 
 #endif
 /* returns 0 */ ;
 
int res= 0 ;
 
 #ifdef _WIN32
 
 res  =  closesocket ( s .sockfd ) ;
 
 /* returns 0 */ #else
 
 res  =  close ( s .sockfd ) ;
 
 #endif
 /* returns 0 */ ;
 
 if ( res < 0 ) {
 
return  v_error ( tos2((byte*)"socket: close failed") ) ;
 
 }
 ;
 
int tmp43 = OPTION_CAST(int)( 0); return opt_ok(&tmp43, sizeof(int)) ;
 
 
 }
 /* returns 0 */ void net__Socket_write(net__Socket s, string str) {
 
string line= _STR("%.*s\r\n", str.len, str.str) ;
 
 write ( s .sockfd ,  line .str ,  line .len ) ;
 
 
 }
 /* returns 0 */ string net__Socket_read_line(net__Socket s) {
 
string res= tos2((byte*)"") ;
 
 while (1) { 
 
 #ifdef VDEBUG
 
 println ( tos2((byte*)".") ) ;
 
 #endif
 /* returns 0 */ ;
 
byte* buf= v_malloc ( net__MAX_READ ) ;
 
int n= ((int)( recv ( s .sockfd ,  buf ,  net__MAX_READ - 1 ,  0 ) ) ) ;
 
 #ifdef VDEBUG
 
printf( "numbytes=%d\n", n ) ;
 
 #endif
 /* returns 0 */ ;
 
 if ( n == - 1 ) {
 
 #ifdef VDEBUG
 
 println ( tos2((byte*)"recv failed") ) ;
 
 #endif
 /* returns 0 */ ;
 
return  tos2((byte*)"") ;
 
 }
 ;
 
 if ( n == 0 ) {
 
 break
 ;
 
 }
 ;
 
 buf [/*ptr*/ n ]/*rbyte 1*/  =  '\0' ;
 
string line= (tos2( buf ) ) ;
 
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
 /* returns 0 */ /* returns 0 */ /* returns 0 */ /* returns 0 */ /* returns 0 */ void os__todo_remove() {
 
 
 }
 /* returns 0 */ array_string os__init_os_args(int argc, byteptr* argv) {
 
array_string args=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
 #ifdef _WIN32
 
voidptr* args_list= ((voidptr*)( 0 ) ) ;
 
int args_count= 0 ;
 
 args_list  =  CommandLineToArgvW ( GetCommandLine ( ) ,  & /*vvar*/  args_count ) ;
 
 for (
int i= 0  ;  i < args_count  ;  i ++ ) { 
 
 
_PUSH(& args , ( string_from_wide ( ((u16*)( args_list [/*ptr*/ i ]/*rvoidptr 1*/ ) ) ) ), tmp5, string) ;
 
 }
 ;
 
 LocalFree ( args_list ) ;
 
 /* returns 0 */ #else
 
 for (
int i= 0  ;  i < argc  ;  i ++ ) { 
 
 
_PUSH(& args , ( (tos2( argv [/*ptr*/ i ]/*rbyteptr 0*/ ) ) ), tmp7, string) ;
 
 }
 ;
 
 #endif
 /* returns 0 */ ;
 
return  args ;
 
 
 }
 /* returns 0 */ array_string os__parse_windows_cmd_line(byte* cmd) {
 
string s= (tos2( cmd ) ) ;
 
return  string_split( s , tos2((byte*)" ") ) ;
 
 
 }
 /* returns 0 */ Option_string os__read_file(string path) {
 
string mode= tos2((byte*)"rb") ;
  
 struct /*c struct init*/ 

FILE* fp= ALLOC_INIT(FILE, { EMPTY_STRUCT_INIT } ) ;
 
 #ifdef _WIN32
 
 fp  =  _wfopen ( string_to_wide( path ) ,  string_to_wide( mode ) ) ;
 
 /* returns 0 */ #else
 
byte* cpath= path .str ;
 
 fp  =  fopen ( cpath ,  mode .str ) ;
 
 #endif
 /* returns 0 */ ;
 
 if ( isnil ( fp ) ) {
 
return  v_error ( _STR("failed to open file \"%.*s\"", path.len, path.str) ) ;
 
 }
 ;
 
 fseek ( fp ,  0 ,  SEEK_END ) ;
 
int fsize= ftell ( fp ) ;
 
 rewind ( fp ) ;
 
byte* str= v_malloc ( fsize + 1 ) ;
 
 fread ( str ,  fsize ,  1 ,  fp ) ;
 
 fclose ( fp ) ;
 
 str [/*ptr*/ fsize ]/*rbyte 1*/  =  0 ;
 
string tmp14 = OPTION_CAST(string)( (tos( str ,  fsize ) )); return opt_ok(&tmp14, sizeof(string)) ;
 
 
 }
 /* returns 0 */ int os__file_size(string path) {
  
 struct /*c struct init*/ 

stat s= (struct stat) { .st_size = 0 , .st_mode = 0 , .st_mtime = 0 } ;
 
 #ifdef _WIN32
 
 _wstat ( string_to_wide( path ) ,  & /*vvar*/  s ) ;
 
 /* returns 0 */ #else
 
 stat ( path .str ,  & /*vvar*/  s ) ;
 
 #endif
 /* returns 0 */ ;
 
return  s .st_size ;
 
 
 }
 /* returns 0 */ void os__mv(string old, string new) {
 
 #ifdef _WIN32
 
 _wrename ( string_to_wide( old ) ,  string_to_wide( new ) ) ;
 
 /* returns 0 */ #else
 
 rename ( old .str ,  new .str ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ array_string os__read_lines(string path) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
int buf_len= 1024 ;
 
byte* buf= v_malloc ( buf_len ) ;
 
string mode= tos2((byte*)"rb") ;
  
 struct /*c struct init*/ 

FILE* fp= ALLOC_INIT(FILE, { EMPTY_STRUCT_INIT } ) ;
 
 #ifdef _WIN32
 
 fp  =  _wfopen ( string_to_wide( path ) ,  string_to_wide( mode ) ) ;
 
 /* returns 0 */ #else
 
 fp  =  fopen ( path .str ,  mode .str ) ;
 
 #endif
 /* returns 0 */ ;
 
 if ( isnil ( fp ) ) {
 
return  res ;
 
 }
 ;
 
int buf_index= 0 ;
 
 while ( fgets ( buf + buf_index ,  buf_len - buf_index ,  fp ) != 0 ) {
 
 
int len= strlen ( buf ) ;
 
 if ( len == buf_len - 1  &&  buf [/*ptr*/ len - 1 ]/*rbyte 1*/ != 10 ) {
 
 buf_len  *=  2 ;
 
 buf  =  realloc ( buf ,  buf_len ) ;
 
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
 
_PUSH(& res , ( tos_clone ( buf ) ), tmp23, string) ;
 
 buf_index  =  0 ;
 
 }
 ;
 
 fclose ( fp ) ;
 
return  res ;
 
 
 }
 /* returns 0 */ array_ustring os__read_ulines(string path) {
 
array_string lines= os__read_lines ( path ) ;
 
array_ustring ulines=new_array_from_c_array(0, 0, sizeof(ustring), (ustring[]) {EMPTY_STRUCT_INIT   }) ;
 
 array_string tmp26 =  lines;
 ;
for (int tmp27 = 0; tmp27 < tmp26 .len; tmp27 ++) {
 string myline = ((string *) tmp26.data)[tmp27];
 
 
_PUSH(& ulines , ( string_ustring( myline ) ), tmp28, ustring) ;
 
 }
 ;
 
return  ulines ;
 
 
 }
 /* returns 0 */ Option_os__File os__open(string path) {
 
os__File file= (struct os__File) { .cfile = 0 } ;
 
 #ifdef _WIN32
 
u16* wpath= string_to_wide( path ) ;
 
string mode= tos2((byte*)"rb") ;
 
 file  =  (os__File) { .cfile =  _wfopen ( wpath ,  string_to_wide( mode ) ) } ;
 
 /* returns 0 */ #else
 
byte* cpath= path .str ;
 
 file  =  (os__File) { .cfile =  fopen ( cpath ,  "rb" ) } ;
 
 #endif
 /* returns 0 */ ;
 
 if ( isnil ( file .cfile ) ) {
 
return  v_error ( _STR("failed to open file \"%.*s\"", path.len, path.str) ) ;
 
 }
 ;
 
os__File tmp33 = OPTION_CAST(os__File)( file); return opt_ok(&tmp33, sizeof(os__File)) ;
 
 
 }
 /* returns 0 */ Option_os__File os__create(string path) {
 
os__File file= (os__File) { .cfile = 0 } ;
 
 #ifdef _WIN32
 
u16* wpath= string_to_wide( string_replace( path , tos2((byte*)"/") , tos2((byte*)"\\") ) ) ;
 
string mode= tos2((byte*)"wb") ;
 
 file  =  (os__File) { .cfile =  _wfopen ( wpath ,  string_to_wide( mode ) ) } ;
 
 /* returns 0 */ #else
 
byte* cpath= path .str ;
 
 file  =  (os__File) { .cfile =  fopen ( cpath ,  "wb" ) } ;
 
 #endif
 /* returns 0 */ ;
 
 if ( isnil ( file .cfile ) ) {
 
return  v_error ( _STR("failed to create file \"%.*s\"", path.len, path.str) ) ;
 
 }
 ;
 
os__File tmp38 = OPTION_CAST(os__File)( file); return opt_ok(&tmp38, sizeof(os__File)) ;
 
 
 }
 /* returns 0 */ Option_os__File os__open_append(string path) {
 
os__File file= (os__File) { .cfile = 0 } ;
 
 #ifdef _WIN32
 
u16* wpath= string_to_wide( string_replace( path , tos2((byte*)"/") , tos2((byte*)"\\") ) ) ;
 
string mode= tos2((byte*)"ab") ;
 
 file  =  (os__File) { .cfile =  _wfopen ( wpath ,  string_to_wide( mode ) ) } ;
 
 /* returns 0 */ #else
 
byte* cpath= path .str ;
 
 file  =  (os__File) { .cfile =  fopen ( cpath ,  "ab" ) } ;
 
 #endif
 /* returns 0 */ ;
 
 if ( isnil ( file .cfile ) ) {
 
return  v_error ( _STR("failed to create(append) file \"%.*s\"", path.len, path.str) ) ;
 
 }
 ;
 
os__File tmp43 = OPTION_CAST(os__File)( file); return opt_ok(&tmp43, sizeof(os__File)) ;
 
 
 }
 /* returns 0 */ void os__File_write(os__File f, string s) {
 
 fputs ( s .str ,  f .cfile ) ;
 
 
 }
 /* returns 0 */ void os__File_write_bytes(os__File f, void* data, int size) {
 
 fwrite ( data ,  1 ,  size ,  f .cfile ) ;
 
 
 }
 /* returns 0 */ void os__File_write_bytes_at(os__File f, void* data, int size, int pos) {
 
 fseek ( f .cfile ,  pos ,  SEEK_SET ) ;
 
 fwrite ( data ,  1 ,  size ,  f .cfile ) ;
 
 fseek ( f .cfile ,  0 ,  SEEK_END ) ;
 
 
 }
 /* returns 0 */ void os__File_writeln(os__File f, string s) {
 
 fputs ( s .str ,  f .cfile ) ;
 
 fputs ( "\n" ,  f .cfile ) ;
 
 
 }
 /* returns 0 */ void os__File_flush(os__File f) {
 
 fflush ( f .cfile ) ;
 
 
 }
 /* returns 0 */ void os__File_close(os__File f) {
 
 fclose ( f .cfile ) ;
 
 
 }
 /* returns 0 */ FILE* os__popen(string path) {
 
 #ifdef _WIN32
 
string mode= tos2((byte*)"rb") ;
 
u16* wpath= string_to_wide( path ) ;
 
return  _wpopen ( wpath ,  string_to_wide( mode ) ) ;
 
 /* returns 0 */ #else
 
byte* cpath= path .str ;
 
return  popen ( cpath ,  "r" ) ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ int os__pclose(FILE* f) {
 
 #ifdef _WIN32
 
return  _pclose ( f ) ;
 
 /* returns 0 */ #else
 
return  pclose ( f ) ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ Option_os__Result os__exec(string cmd) {
 
string pcmd= _STR("%.*s 2>&1", cmd.len, cmd.str) ;
 
FILE* f= os__popen ( pcmd ) ;
 
 if ( isnil ( f ) ) {
 
return  v_error ( _STR("exec(\"%.*s\") failed", cmd.len, cmd.str) ) ;
 
 }
 ;
 
byte buf  [1000 ]= STRUCT_DEFAULT_VALUE ;
 
string res= tos2((byte*)"") ;
 
 while ( fgets ( buf ,  1000 ,  f ) != 0 ) {
 
 
 res = string_add(res,  tos ( buf , strlen ( buf ) ) ) ;
 
 }
 ;
 
 res  =  string_trim_space( res ) ;
 
int exit_code= os__pclose ( f ) / 256 ;
 
os__Result tmp52 = OPTION_CAST(os__Result)( (os__Result) { .output =  res , .exit_code =  exit_code }); return opt_ok(&tmp52, sizeof(os__Result)) ;
 
 
 }
 /* returns 0 */ int os__system(string cmd) {
 
int ret= ((int)( 0 ) ) ;
 
 #ifdef _WIN32
 
 ret  =  _wsystem ( string_to_wide( cmd ) ) ;
 
 /* returns 0 */ #else
 
 ret  =  system ( cmd .str ) ;
 
 #endif
 /* returns 0 */ ;
 
 if ( ret == - 1 ) {
 
 os__print_c_errno ( ) ;
 
 }
 ;
 
return  ret ;
 
 
 }
 /* returns 0 */ string os__getenv(string key) {
 
 #ifdef _WIN32
 
void* s= _wgetenv ( string_to_wide( key ) ) ;
 
 if ( isnil ( s ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_from_wide ( s ) ;
 
 /* returns 0 */ #else
 
byte* s= getenv ( key .str ) ;
 
 if ( isnil ( s ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  (tos2( s ) ) ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ int os__setenv(string name, string value, bool overwrite) {
 
 #ifdef _WIN32
 
string format= _STR("%.*s=%.*s", name.len, name.str, value.len, value.str) ;
 
 if ( overwrite ) {
 
return  _putenv ( format .str ) ;
 
 }
 ;
 
return  - 1 ;
 
 /* returns 0 */ #else
 
return  setenv ( name .str ,  value .str ,  overwrite ) ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ int os__unsetenv(string name) {
 
 #ifdef _WIN32
 
string format= _STR("%.*s=", name.len, name.str) ;
 
return  _putenv ( format .str ) ;
 
 /* returns 0 */ #else
 
return  unsetenv ( name .str ) ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ bool os__file_exists(string _path) {
 
 #ifdef _WIN32
 
string path= string_replace( _path , tos2((byte*)"/") , tos2((byte*)"\\") ) ;
 
return  _waccess ( string_to_wide( path ) ,  0 ) != - 1 ;
 
 /* returns 0 */ #else
 
return  access ( _path .str ,  0 ) != - 1 ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ void os__rm(string path) {
 
 #ifdef _WIN32
 
 _wremove ( string_to_wide( path ) ) ;
 
 /* returns 0 */ #else
 
 remove ( path .str ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ void os__rmdir(string path) {
 
 #ifndef _WIN32
 
 rmdir ( path .str ) ;
 
 /* returns 0 */ #else
 
 RemoveDirectory ( string_to_wide( path ) ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ void os__print_c_errno() {
 
 
 }
 /* returns 0 */ string os__ext(string path) {
 
int pos= string_last_index( path , tos2((byte*)".") ) ;
 
 if ( pos == - 1 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_right( path , pos ) ;
 
 
 }
 /* returns 0 */ string os__dir(string path) {
 
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
 /* returns 0 */ string os__path_sans_ext(string path) {
 
int pos= string_last_index( path , tos2((byte*)".") ) ;
 
 if ( pos == - 1 ) {
 
return  path ;
 
 }
 ;
 
return  string_left( path , pos ) ;
 
 
 }
 /* returns 0 */ string os__basedir(string path) {
 
int pos= string_last_index( path , tos2((byte*)"/") ) ;
 
 if ( pos == - 1 ) {
 
return  path ;
 
 }
 ;
 
return  string_left( path , pos + 1 ) ;
 
 
 }
 /* returns 0 */ string os__filename(string path) {
 
return  string_all_after( path , tos2((byte*)"/") ) ;
 
 
 }
 /* returns 0 */ string os__get_line() {
 
string str= os__get_raw_line ( ) ;
 
 #ifdef _WIN32
 
return  string_trim_right( str , tos2((byte*)"\r\n") ) ;
 
 /* returns 0 */ #else
 
return  string_trim_right( str , tos2((byte*)"\n") ) ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ string os__get_raw_line() {
 
 #ifdef _WIN32
 
int max= 512 ;
 
u16* buf= ((u16*)( v_malloc ( max * 2 ) ) ) ;
 
void* h_input= GetStdHandle ( os__STD_INPUT_HANDLE ) ;
 
 if ( h_input == os__INVALID_HANDLE_VALUE ) {
 
 v_panic ( tos2((byte*)"get_raw_line() error getting input handle.") ) ;
 
 }
 ;
 
int nr_chars= 0 ;
 
 ReadConsole ( h_input ,  buf ,  max ,  & /*vvar*/  nr_chars ,  0 ) ;
 
 if ( nr_chars == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_from_wide2 ( buf , nr_chars ) ;
 
 /* returns 0 */ #else
 
u64 max= ((u64)( 256 ) ) ;
 
byte* buf= v_malloc ( ((int)( max ) ) ) ;
 
int nr_chars= getline ( & /*vvar*/  buf ,  & /*vvar*/  max ,  stdin ) ;
 
 if ( nr_chars == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  (tos( buf ,  nr_chars ) ) ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ array_string os__get_lines() {
 
string line= tos2((byte*)"") ;
 
array_string inputstr=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
 while (1) { 
 
 line  =  os__get_line ( ) ;
 
 if ( ( line .len <= 0 ) ) {
 
 break
 ;
 
 }
 ;
 
 line  =  string_trim_space( line ) ;
 
_PUSH(& inputstr , ( line ), tmp73, string) ;
 
 }
 ;
 
return  inputstr ;
 
 
 }
 /* returns 0 */ string os__get_lines_joined() {
 
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
 /* returns 0 */ string os__user_os() {
 
 #ifdef __linux__
 
return  tos2((byte*)"linux") ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __APPLE__
 
return  tos2((byte*)"mac") ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef _WIN32
 
return  tos2((byte*)"windows") ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __FreeBSD__
 
return  tos2((byte*)"freebsd") ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __OpenBSD__
 
return  tos2((byte*)"openbsd") ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __NetBSD__
 
return  tos2((byte*)"netbsd") ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __DragonFly__
 
return  tos2((byte*)"dragonfly") ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef _MSC_VER
 
return  tos2((byte*)"windows") ;
 
 #endif
 /* returns 0 */ ;
 
return  tos2((byte*)"unknown") ;
 
 
 }
 /* returns 0 */ string os__home_dir() {
 
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
 /* returns 0 */ ;
 
 home = string_add(home,  tos2((byte*)"/") ) ;
 
return  home ;
 
 
 }
 /* returns 0 */ void os__write_file(string path, string text) {
 
Option_os__File tmp78 =  os__create ( path ) ;
 if (!tmp78 .ok) {
 string err = tmp78 . error;
 
 return ;
 
 }
 os__File f = *(os__File*) tmp78 . data;
 ;
 
 os__File_write( f , text ) ;
 
 os__File_close( f ) ;
 
 
 }
 /* returns 0 */ void os__clear() {
 
 printf ( "\x1b[2J" ) ;
 
 printf ( "\x1b[H" ) ;
 
 
 }
 /* returns 0 */ void os__on_segfault(void* f) {
 
 #ifdef _WIN32
 
 return ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __APPLE__
  
 struct /*c struct init*/ 

sigaction sa= (struct sigaction) { .sa_mask = 0 , .sa_sigaction = 0 , .sa_flags = 0 } ;
 
 memset ( & /*vvar*/  sa ,  0 ,  sizeof( sigaction) ) ;
 
 sigemptyset ( & /*vvar*/  sa .sa_mask ) ;
 
 sa .sa_sigaction  =  f ;
 
 sa .sa_flags  =  SA_SIGINFO ;
 
 sigaction ( SIGSEGV ,  & /*vvar*/  sa ,  0 ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ string os__executable() {
 
 #ifdef __linux__
 
byte* result= v_malloc ( os__MAX_PATH ) ;
 
int count= ((int)( readlink ( "/proc/self/exe" ,  result ,  os__MAX_PATH ) ) ) ;
 
 if ( count < 0 ) {
 
 v_panic ( tos2((byte*)"error reading /proc/self/exe to get exe path") ) ;
 
 }
 ;
 
return  (tos( result ,  count ) ) ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef _WIN32
 
int max= 512 ;
 
u16* result= ((u16*)( v_malloc ( max * 2 ) ) ) ;
 
int len= ((int)( GetModuleFileName ( 0 ,  result ,  max ) ) ) ;
 
return  string_from_wide2 ( result , len ) ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __APPLE__
 
byte* result= v_malloc ( os__MAX_PATH ) ;
 
void* pid= getpid ( ) ;
 
void* ret= proc_pidpath ( pid ,  result ,  os__MAX_PATH ) ;
 
 if ( ret <= 0 ) {
 
 println ( tos2((byte*)"os.executable() failed") ) ;
 
return  tos2((byte*)".") ;
 
 }
 ;
 
return  (tos2( result ) ) ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __FreeBSD__
 
byte* result= v_malloc ( os__MAX_PATH ) ;
 
array_int mib=new_array_from_c_array(4, 4, sizeof(int), (int[]) {  1 ,  14 ,  12 ,  - 1  }) ;
 
int size= os__MAX_PATH ;
 
 sysctl ( mib .data ,  4 ,  result ,  & /*vvar*/  size ,  0 ,  0 ) ;
 
return  (tos2( result ) ) ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __OpenBSD__
 
return  ( *(string*) array__get( os__args , 0) ) ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __NetBSD__
 
byte* result= v_malloc ( os__MAX_PATH ) ;
 
int count= ((int)( readlink ( "/proc/curproc/exe" ,  result ,  os__MAX_PATH ) ) ) ;
 
 if ( count < 0 ) {
 
 v_panic ( tos2((byte*)"error reading /proc/curproc/exe to get exe path") ) ;
 
 }
 ;
 
return  (tos( result ,  count ) ) ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __DragonFly__
 
byte* result= v_malloc ( os__MAX_PATH ) ;
 
int count= ((int)( readlink ( "/proc/curproc/file" ,  result ,  os__MAX_PATH ) ) ) ;
 
 if ( count < 0 ) {
 
 v_panic ( tos2((byte*)"error reading /proc/curproc/file to get exe path") ) ;
 
 }
 ;
 
return  (tos( result ,  count ) ) ;
 
 #endif
 /* returns 0 */ ;
 
return  tos2((byte*)".") ;
 
 
 }
 /* returns 0 */ bool os__is_dir(string path) {
 
 #ifdef _WIN32
 
return  os__dir_exists ( path ) ;
 
 /* returns 0 */ #else
  
 struct /*c struct init*/ 

stat statbuf= (struct stat) { .st_size = 0 , .st_mode = 0 , .st_mtime = 0 } ;
 
byte* cstr= path .str ;
 
 if ( stat ( cstr ,  & /*vvar*/  statbuf ) != 0 ) {
 
return  0 ;
 
 }
 ;
 
return  ( statbuf .st_mode & S_IFMT ) == S_IFDIR ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ void os__chdir(string path) {
 
 #ifdef _WIN32
 
 _wchdir ( string_to_wide( path ) ) ;
 
 /* returns 0 */ #else
 
 chdir ( path .str ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ string os__getwd() {
 
 #ifdef _WIN32
 
int max= 512 ;
 
u16* buf= ((u16*)( v_malloc ( max * 2 ) ) ) ;
 
 if ( _wgetcwd ( buf ,  max ) == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_from_wide ( buf ) ;
 
 /* returns 0 */ #else
 
byte* buf= v_malloc ( 512 ) ;
 
 if ( getcwd ( buf ,  512 ) == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  (tos2( buf ) ) ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ string os__realpath(string fpath) {
 
byte* fullpath= v_malloc ( os__MAX_PATH ) ;
 
int res= 0 ;
 
 #ifdef _WIN32
 
 res  =  ((int)( _fullpath ( fullpath ,  fpath .str ,  os__MAX_PATH ) ) ) ;
 
 /* returns 0 */ #else
 
 res  =  ((int)( realpath ( fpath .str ,  fullpath ) ) ) ;
 
 #endif
 /* returns 0 */ ;
 
 if ( res != 0 ) {
 
return  (tos( fullpath ,  strlen ( fullpath ) ) ) ;
 
 }
 ;
 
return  fpath ;
 
 
 }
 /* returns 0 */ array_string os__walk_ext(string path, string ext) {
 
 if ( ! os__is_dir ( path ) ) {
 
return new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
 }
 ;
 
array_string files= os__ls ( path ) ;
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
 array_string tmp106 =  files ;
 ;
for (int i = 0; i < tmp106 .len; i ++) {
 string file = ((string *) tmp106 . data)[i];
 
 
 if ( string_starts_with( file , tos2((byte*)".") ) ) {
 
 continue
 ;
 
 }
 ;
 
string p=string_add(string_add( path , tos2((byte*)"/") ) , file ) ;
 
 if ( os__is_dir ( p ) ) {
 
_PUSH_MANY(& res , ( os__walk_ext ( p , ext ) ), tmp108, array_string) ;
 
 }
  else  if ( string_ends_with( file , ext ) ) {
 
_PUSH(& res , ( p ), tmp109, string) ;
 
 }
 ;
 
 }
 ;
 
return  res ;
 
 
 }
 /* returns 0 */ void os__signal(int signum, void* handler) {
 
 signal ( signum ,  handler ) ;
 
 
 }
 /* returns 0 */ int os__fork() {
 
 #ifndef _WIN32
 
void* pid= fork ( ) ;
 
return  pid ;
 
 #endif
 /* returns 0 */ ;
 
 v_panic ( tos2((byte*)"os.fork not supported in windows") ) ;
 
 
 }
 /* returns 0 */ int os__wait() {
 
 #ifndef _WIN32
 
void* pid= wait ( 0 ) ;
 
return  pid ;
 
 #endif
 /* returns 0 */ ;
 
 v_panic ( tos2((byte*)"os.wait not supported in windows") ) ;
 
 
 }
 /* returns 0 */ int os__file_last_mod_unix(string path) {
  
 struct /*c struct init*/ 

stat attr= (struct stat) { .st_size = 0 , .st_mode = 0 , .st_mtime = 0 } ;
 
 stat ( path .str ,  & /*vvar*/  attr ) ;
 
return  attr .st_mtime ;
 
 
 }
 /* returns 0 */ void os__log(string s) {
 
 
 }
 /* returns 0 */ void os__flush_stdout() {
 
 fflush ( stdout ) ;
 
 
 }
 /* returns 0 */ void os__print_backtrace() {
 
 
 }
 /* returns 0 */ string os__get_error_msg(int code) {
 
void* _ptr_text= strerror ( code ) ;
 
 if ( _ptr_text == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  tos ( _ptr_text , strlen ( _ptr_text ) ) ;
 
 
 }
 /* returns 0 */ array_string os__ls(string path) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
void* dir= opendir ( path .str ) ;
 
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
 
string name= tos_clone ( ent ->d_name ) ;
 
 if (string_ne( name , tos2((byte*)".") )  && string_ne( name , tos2((byte*)"..") )  && string_ne( name , tos2((byte*)"") ) ) {
 
_PUSH(& res , ( name ), tmp6, string) ;
 
 }
 ;
 
 }
 ;
 
 closedir ( dir ) ;
 
return  res ;
 
 
 }
 /* returns 0 */ bool os__dir_exists(string path) {
 
void* dir= opendir ( path .str ) ;
 
bool res= ! isnil ( dir ) ;
 
 if ( res ) {
 
 closedir ( dir ) ;
 
 }
 ;
 
return  res ;
 
 
 }
 /* returns 0 */ void os__mkdir(string path) {
 
 mkdir ( path .str ,  511 ) ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__error_msg(string message, string val) {
 
string msg= _STR("net.urllib: %.*s", message.len, message.str) ;
 
 if (string_ne( val , tos2((byte*)"") ) ) {
 
 msg  =  _STR("%.*s (%.*s)", msg.len, msg.str, val.len, val.str) ;
 
 }
 ;
 
return  msg ;
 
 
 }
 /* returns 0 */ bool net_dot_urllib__should_escape(byte c, net_dot_urllib__EncodingMode mode) {
 
 if ( ( 'a' <= c  &&  c <= 'z' )  ||  ( 'A' <= c  &&  c <= 'Z' )  ||  ( '0' <= c  &&  c <= '9' ) ) {
 
return  0 ;
 
 }
 ;
 
 if ( mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_host  ||  mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_zone ) {
 
 if ( ( c ==  '!' ) ||   ( c ==  '$' ) ||   ( c ==  '&' ) ||   ( c ==  '\\' ) ||   ( c ==  '(' ) ||   ( c ==  ')' ) ||   ( c ==  '*' ) ||   ( c ==  '+' ) ||   ( c ==  ',' ) ||   ( c ==  ';' ) ||   ( c ==  '=' ) ||   ( c ==  ':' ) ||   ( c ==  '[' ) ||   ( c ==  ']' ) ||   ( c ==  '<' ) ||   ( c ==  '>' ) ||   ( c ==  '"' )) { /* case */
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
 if ( ( c ==  '-' ) ||   ( c ==  '_' ) ||   ( c ==  '.' ) ||   ( c ==  '~' )) { /* case */
 
return  0 ;
 
 }
 else  if ( ( c ==  '$' ) ||   ( c ==  '&' ) ||   ( c ==  '+' ) ||   ( c ==  ',' ) ||   ( c ==  '/' ) ||   ( c ==  ':' ) ||   ( c ==  ';' ) ||   ( c ==  '=' ) ||   ( c ==  '?' ) ||   ( c ==  '@' )) { /* case */
 
 if ( ( mode ==  net_dot_urllib__net_dot_urllib__EncodingMode_encode_path )) { /* case */
 
return  c == '?' ;
 
 }
 else  if ( ( mode ==  net_dot_urllib__net_dot_urllib__EncodingMode_encode_path_segment )) { /* case */
 
return  c == '/'  ||  c == ';'  ||  c == ','  ||  c == '?' ;
 
 }
 else  if ( ( mode ==  net_dot_urllib__net_dot_urllib__EncodingMode_encode_user_password )) { /* case */
 
return  c == '@'  ||  c == '/'  ||  c == '?'  ||  c == ':' ;
 
 }
 else  if ( ( mode ==  net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component )) { /* case */
 
return  1 ;
 
 }
 else  if ( ( mode ==  net_dot_urllib__net_dot_urllib__EncodingMode_encode_fragment )) { /* case */
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
 if ( mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_fragment ) {
 
 if ( ( c ==  '!' ) ||   ( c ==  '(' ) ||   ( c ==  ')' ) ||   ( c ==  '*' )) { /* case */
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
return  1 ;
 
 
 }
 /* returns 0 */ Option_string net_dot_urllib__query_unescape(string s) {
 
Option_string tmp2 = OPTION_CAST(Option_string)( net_dot_urllib__unescape ( s , net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component )); return opt_ok(&tmp2, sizeof(string)) ;
 
 
 }
 /* returns 0 */ Option_string net_dot_urllib__path_unescape(string s) {
 
Option_string tmp3 = OPTION_CAST(Option_string)( net_dot_urllib__unescape ( s , net_dot_urllib__net_dot_urllib__EncodingMode_encode_path_segment )); return opt_ok(&tmp3, sizeof(string)) ;
 
 
 }
 /* returns 0 */ Option_string net_dot_urllib__unescape(string s_, net_dot_urllib__EncodingMode mode) {
 
string s= s_ ;
 
int n= 0 ;
 
bool has_plus= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  ) { 
 
 
byte x= string_at( s , i) ;
 
 if ( ( x ==  '%' )) { /* case */
 
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
 else  if ( ( x ==  '+' )) { /* case */
 
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
 
 if ( ( x ==  '%' )) { /* case */
 
 strings__Builder_write(& /* ? */ t , byte_str( ((byte)( net_dot_urllib__unhex ( string_at( s , i + 1) )  <<  ((byte)( 4 ) ) | net_dot_urllib__unhex ( string_at( s , i + 2) ) ) ) ) ) ;
 
 i  +=  2 ;
 
 }
 else  if ( ( x ==  '+' )) { /* case */
 
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
 /* returns 0 */ string net_dot_urllib__query_escape(string s) {
 
return  net_dot_urllib__escape ( s , net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component ) ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__path_escape(string s) {
 
return  net_dot_urllib__escape ( s , net_dot_urllib__net_dot_urllib__EncodingMode_encode_path_segment ) ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__escape(string s, net_dot_urllib__EncodingMode mode) {
 
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
  
 byte tmp45 =  ((byte)( 0 ) ); 

array_byte buf= array_repeat(&tmp45,  64 , sizeof(byte) ) ;
 
array_byte t=new_array_from_c_array(0, 0, sizeof(byte), (byte[]) {EMPTY_STRUCT_INIT   }) ;
 
int required= s .len + 2 * hex_count ;
 
 if ( required <= buf .len ) {
 
 t  =  array_left( buf , required ) ;
 
 }
  else { 
  
 byte tmp49 =  ((byte)( 0 ) ); 

 t  =  array_repeat(&tmp49,  required , sizeof(byte) ) ;
 
 }
 ;
 
 if ( hex_count == 0 ) {
 
 copy ( t , string_bytes( s ) ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( string_at( s , i) == ' ' ) {
  
 byte tmp53 =  '+'; 

array_set(&/*q*/ t , i , & tmp53) ;
 
 }
 ;
 
 }
 ;
 
return  (tos2( t .data ) ) ;
 
 }
 ;
 
string ctab= tos2((byte*)"0123456789ABCDEF") ;
 
int j= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
byte c1= string_at( s , i) ;
 
 if ( c1 == ' '  &&  mode == net_dot_urllib__net_dot_urllib__EncodingMode_encode_query_component ) {
  
 byte tmp60 =  '+'; 

array_set(&/*q*/ t , j , & tmp60) ;
 
 j ++ ;
 
 }
  else  if ( net_dot_urllib__should_escape ( c1 , mode ) ) {
  
 byte tmp61 =  '%'; 

array_set(&/*q*/ t , j , & tmp61) ;
  
 byte tmp64 =  string_at( ctab , c1  >>  4); 

array_set(&/*q*/ t , j + 1 , & tmp64) ;
  
 byte tmp67 =  string_at( ctab , c1 & 15); 

array_set(&/*q*/ t , j + 2 , & tmp67) ;
 
 j  +=  3 ;
 
 }
  else { 
  
 byte tmp70 =  string_at( s , i); 

array_set(&/*q*/ t , j , & tmp70) ;
 
 j ++ ;
 
 }
 ;
 
 }
 ;
 
return  (tos2( t .data ) ) ;
 
 
 }
 /* returns 0 */ net_dot_urllib__Userinfo* net_dot_urllib__user(string username) {
 
return  ALLOC_INIT(net_dot_urllib__Userinfo, { .username =  username , .password =  tos2((byte*)"") , .password_set =  0 } ) ;
 
 
 }
 /* returns 0 */ net_dot_urllib__Userinfo* net_dot_urllib__user_password(string username, string password) {
 
return  ALLOC_INIT(net_dot_urllib__Userinfo, { username , password , 1 } ) ;
 
 
 }
 /* returns 0 */ bool net_dot_urllib__Userinfo_empty(net_dot_urllib__Userinfo* u) {
 
return string_eq( u ->username , tos2((byte*)"") )  && string_eq( u ->password , tos2((byte*)"") ) ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__Userinfo_string(net_dot_urllib__Userinfo* u) {
 
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
 /* returns 0 */ Option_array_string net_dot_urllib__split_by_scheme(string rawurl) {
 
 for (
int i= 0  ;  i < rawurl .len  ;  i ++ ) { 
 
 
byte c= string_at( rawurl , i) ;
 
 if ( ( 'a' <= c  &&  c <= 'z' )  ||  ( 'A' <= c  &&  c <= 'Z' ) ) {
 
 }
  else  if ( ( '0' <= c  &&  c <= '9' )  ||  ( c == '+'  ||  c == '-'  ||  c == '.' ) ) {
 
 if ( i == 0 ) {
 
array_string tmp76 = OPTION_CAST(array_string)(new_array_from_c_array(2, 2, sizeof(string), (string[]) {  tos2((byte*)"") ,  rawurl  })); return opt_ok(&tmp76, sizeof(array_string)) ;
 
 }
 ;
 
 }
  else  if ( c == ':' ) {
 
 if ( i == 0 ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"missing protocol scheme") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
array_string tmp77 = OPTION_CAST(array_string)(new_array_from_c_array(2, 2, sizeof(string), (string[]) {  string_left( rawurl , i ) ,  string_right( rawurl , i + 1 )  })); return opt_ok(&tmp77, sizeof(array_string)) ;
 
 }
  else { 
 
array_string tmp78 = OPTION_CAST(array_string)(new_array_from_c_array(2, 2, sizeof(string), (string[]) {  tos2((byte*)"") ,  rawurl  })); return opt_ok(&tmp78, sizeof(array_string)) ;
 
 }
 ;
 
 }
 ;
 
array_string tmp79 = OPTION_CAST(array_string)(new_array_from_c_array(2, 2, sizeof(string), (string[]) {  tos2((byte*)"") ,  rawurl  })); return opt_ok(&tmp79, sizeof(array_string)) ;
 
 
 }
 /* returns 0 */ Option_string net_dot_urllib__get_scheme(string rawurl) {
 
Option_array_string tmp80 =  net_dot_urllib__split_by_scheme ( rawurl ) ;
 if (!tmp80 .ok) {
 string err = tmp80 . error;
 
string tmp81 = OPTION_CAST(string)( err); return opt_ok(&tmp81, sizeof(string)) ;
 
 }
 array_string split = *(array_string*) tmp80 . data;
 ;
 
string tmp84 = OPTION_CAST(string)( ( *(string*) array__get( split , 0) )); return opt_ok(&tmp84, sizeof(string)) ;
 
 
 }
 /* returns 0 */ array_string net_dot_urllib__split(string s, string c, bool cutc) {
 
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
 /* returns 0 */ Option_net_dot_urllib__URL net_dot_urllib__parse(string rawurl) {
 
array_string p= net_dot_urllib__split ( rawurl , tos2((byte*)"#") , 1 ) ;
 
string u= ( *(string*) array__get( p , 0) ) ;
 
string frag= ( *(string*) array__get( p , 1) ) ;
 
Option_net_dot_urllib__URL tmp93 =  net_dot_urllib___parse ( u , 0 ) ;
 if (!tmp93 .ok) {
 string err = tmp93 . error;
 
return  v_error ( net_dot_urllib__error_msg ( net_dot_urllib__err_msg_parse , u ) ) ;
 
 }
 net_dot_urllib__URL url = *(net_dot_urllib__URL*) tmp93 . data;
 ;
 
 if (string_eq( frag , tos2((byte*)"") ) ) {
 
net_dot_urllib__URL tmp94 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp94, sizeof(net_dot_urllib__URL)) ;
 
 }
 ;
 
Option_string tmp95 =  net_dot_urllib__unescape ( frag , net_dot_urllib__net_dot_urllib__EncodingMode_encode_fragment ) ;
 if (!tmp95 .ok) {
 string err = tmp95 . error;
 
return  v_error ( net_dot_urllib__error_msg ( net_dot_urllib__err_msg_parse , u ) ) ;
 
 }
 string f = *(string*) tmp95 . data;
 ;
 
 url .fragment  =  f ;
 
net_dot_urllib__URL tmp96 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp96, sizeof(net_dot_urllib__URL)) ;
 
 
 }
 /* returns 0 */ Option_net_dot_urllib__URL net_dot_urllib__parse_request_uri(string rawurl) {
 
Option_net_dot_urllib__URL tmp97 = OPTION_CAST(Option_net_dot_urllib__URL)( net_dot_urllib___parse ( rawurl , 1 )); return opt_ok(&tmp97, sizeof(net_dot_urllib__URL)) ;
 
 
 }
 /* returns 0 */ Option_net_dot_urllib__URL net_dot_urllib___parse(string rawurl, bool via_request) {
 
 if ( net_dot_urllib__string_contains_ctl_byte ( rawurl ) ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"invalid control character in URL") , rawurl ) ) ;
 
 }
 ;
 
 if (string_eq( rawurl , tos2((byte*)"") )  &&  via_request ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"empty URL") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
net_dot_urllib__URL url= (net_dot_urllib__URL) { .scheme = tos("", 0) , .opaque = tos("", 0) , .user = 0 , .host = tos("", 0) , .path = tos("", 0) , .raw_path = tos("", 0) , .force_query = 0 , .raw_query = tos("", 0) , .fragment = tos("", 0) } ;
 
 if (string_eq( rawurl , tos2((byte*)"*") ) ) {
 
 url .path  =  tos2((byte*)"*") ;
 
net_dot_urllib__URL tmp99 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp99, sizeof(net_dot_urllib__URL)) ;
 
 }
 ;
 
Option_array_string tmp100 =  net_dot_urllib__split_by_scheme ( rawurl ) ;
 if (!tmp100 .ok) {
 string err = tmp100 . error;
 
return  v_error ( err ) ;
 
 }
 array_string p = *(array_string*) tmp100 . data;
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
 
net_dot_urllib__URL tmp111 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp111, sizeof(net_dot_urllib__URL)) ;
 
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
 
Option_net_dot_urllib__ParseAuthorityRes tmp120 =  net_dot_urllib__parse_authority ( authority ) ;
 if (!tmp120 .ok) {
 string err = tmp120 . error;
 
return  v_error ( err ) ;
 
 }
 net_dot_urllib__ParseAuthorityRes a = *(net_dot_urllib__ParseAuthorityRes*) tmp120 . data;
 ;
 
 url .user  =  a .user ;
 
 url .host  =  a .host ;
 
 }
 ;
 
Option_bool tmp121 =  net_dot_urllib__URL_set_path(& /* ? */ url , rest ) ;
 if (!tmp121 .ok) {
 string err = tmp121 . error;
 
return  v_error ( err ) ;
 
 }
 bool _ = *(bool*) tmp121 . data;
 ;
 
net_dot_urllib__URL tmp122 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp122, sizeof(net_dot_urllib__URL)) ;
 
 
 }
 /* returns 0 */ Option_net_dot_urllib__ParseAuthorityRes net_dot_urllib__parse_authority(string authority) {
 
int i= string_last_index( authority , tos2((byte*)"@") ) ;
 
string host= tos2((byte*)"") ;
 
net_dot_urllib__Userinfo* user= net_dot_urllib__user ( tos2((byte*)"") ) ;
 
 if ( i < 0 ) {
 
Option_string tmp126 =  net_dot_urllib__parse_host ( authority ) ;
 if (!tmp126 .ok) {
 string err = tmp126 . error;
 
return  v_error ( err ) ;
 
 }
 string h = *(string*) tmp126 . data;
 ;
 
 host  =  h ;
 
 }
  else { 
 
Option_string tmp127 =  net_dot_urllib__parse_host ( string_right( authority , i + 1 ) ) ;
 if (!tmp127 .ok) {
 string err = tmp127 . error;
 
return  v_error ( err ) ;
 
 }
 string h = *(string*) tmp127 . data;
 ;
 
 host  =  h ;
 
 }
 ;
 
 if ( i < 0 ) {
 
net_dot_urllib__ParseAuthorityRes tmp128 = OPTION_CAST(net_dot_urllib__ParseAuthorityRes)( (net_dot_urllib__ParseAuthorityRes) { .host =  host , .user = 0 , }); return opt_ok(&tmp128, sizeof(net_dot_urllib__ParseAuthorityRes)) ;
 
 }
 ;
 
string userinfo= string_left( authority , i ) ;
 
 if ( ! net_dot_urllib__valid_userinfo ( userinfo ) ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"invalid userinfo") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
 if ( ! string_contains( userinfo , tos2((byte*)":") ) ) {
 
Option_string tmp130 =  net_dot_urllib__unescape ( userinfo , net_dot_urllib__net_dot_urllib__EncodingMode_encode_user_password ) ;
 if (!tmp130 .ok) {
 string err = tmp130 . error;
 
return  v_error ( err ) ;
 
 }
 string u = *(string*) tmp130 . data;
 ;
 
 userinfo  =  u ;
 
 user  =  user ;
 
 ( userinfo ) ; 
 
 }
  else { 
 
array_string parts= net_dot_urllib__split ( userinfo , tos2((byte*)":") , 1 ) ;
 
string username= ( *(string*) array__get( parts , 0) ) ;
 
string password= ( *(string*) array__get( parts , 1) ) ;
 
Option_string tmp138 =  net_dot_urllib__unescape ( username , net_dot_urllib__net_dot_urllib__EncodingMode_encode_user_password ) ;
 if (!tmp138 .ok) {
 string err = tmp138 . error;
 
return  v_error ( err ) ;
 
 }
 string u = *(string*) tmp138 . data;
 ;
 
 username  =  u ;
 
Option_string tmp139 =  net_dot_urllib__unescape ( password , net_dot_urllib__net_dot_urllib__EncodingMode_encode_user_password ) ;
 if (!tmp139 .ok) {
 string err = tmp139 . error;
 
return  v_error ( err ) ;
 
 }
 string p = *(string*) tmp139 . data;
 ;
 
 password  =  p ;
 
 user  =  net_dot_urllib__user_password ( username , password ) ;
 
 }
 ;
 
net_dot_urllib__ParseAuthorityRes tmp140 = OPTION_CAST(net_dot_urllib__ParseAuthorityRes)( (net_dot_urllib__ParseAuthorityRes) { .user =  user , .host =  host }); return opt_ok(&tmp140, sizeof(net_dot_urllib__ParseAuthorityRes)) ;
 
 
 }
 /* returns 0 */ Option_string net_dot_urllib__parse_host(string host) {
 
 if ( string_starts_with( host , tos2((byte*)"[") ) ) {
 
int i= string_last_index( host , tos2((byte*)"]") ) ;
 
 if ( i < 0 ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"missing \']\' in host") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
string colonport= string_right( host , i + 1 ) ;
 
 if ( ! net_dot_urllib__valid_optional_port ( colonport ) ) {
 
return  v_error ( net_dot_urllib__error_msg ( _STR("invalid port %.*s after host ", colonport.len, colonport.str) , tos2((byte*)"") ) ) ;
 
 }
 ;
 
int zone= string_index( string_left( host , i ) , tos2((byte*)"%25") ) ;
 
 if ( zone >= 0 ) {
 
Option_string tmp144 =  net_dot_urllib__unescape ( string_left( host , zone ) , net_dot_urllib__net_dot_urllib__EncodingMode_encode_host ) ;
 if (!tmp144 .ok) {
 string err = tmp144 . error;
 
string tmp145 = OPTION_CAST(string)( err); return opt_ok(&tmp145, sizeof(string)) ;
 
 }
 string host1 = *(string*) tmp144 . data;
 ;
 
Option_string tmp146 =  net_dot_urllib__unescape ( string_substr( host , zone , i ) , net_dot_urllib__net_dot_urllib__EncodingMode_encode_zone ) ;
 if (!tmp146 .ok) {
 string err = tmp146 . error;
 
string tmp147 = OPTION_CAST(string)( err); return opt_ok(&tmp147, sizeof(string)) ;
 
 }
 string host2 = *(string*) tmp146 . data;
 ;
 
Option_string tmp148 =  net_dot_urllib__unescape ( string_right( host , i ) , net_dot_urllib__net_dot_urllib__EncodingMode_encode_host ) ;
 if (!tmp148 .ok) {
 string err = tmp148 . error;
 
string tmp149 = OPTION_CAST(string)( err); return opt_ok(&tmp149, sizeof(string)) ;
 
 }
 string host3 = *(string*) tmp148 . data;
 ;
 
string tmp150 = OPTION_CAST(string)(string_add(string_add( host1 , host2 ) , host3 )); return opt_ok(&tmp150, sizeof(string)) ;
 
 }
 ;
 
 }
 ;
 
Option_string tmp151 =  net_dot_urllib__unescape ( host , net_dot_urllib__net_dot_urllib__EncodingMode_encode_host ) ;
 if (!tmp151 .ok) {
 string err = tmp151 . error;
 
string tmp152 = OPTION_CAST(string)( err); return opt_ok(&tmp152, sizeof(string)) ;
 
 }
 string h = *(string*) tmp151 . data;
 ;
 
string tmp153 = OPTION_CAST(string)( h); return opt_ok(&tmp153, sizeof(string)) ;
 
 
 }
 /* returns 0 */ Option_bool net_dot_urllib__URL_set_path(net_dot_urllib__URL* u, string p) {
 
Option_string tmp154 =  net_dot_urllib__unescape ( p , net_dot_urllib__net_dot_urllib__EncodingMode_encode_path ) ;
 if (!tmp154 .ok) {
 string err = tmp154 . error;
 
return  v_error ( err ) ;
 
 }
 string path = *(string*) tmp154 . data;
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
 
bool tmp156 = OPTION_CAST(bool)( 1); return opt_ok(&tmp156, sizeof(bool)) ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__URL_escaped_path(net_dot_urllib__URL* u) {
 
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
 /* returns 0 */ bool net_dot_urllib__valid_encoded_path(string s) {
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
byte x= string_at( s , i) ;
 
 if ( ( x ==  '!' ) ||   ( x ==  '$' ) ||   ( x ==  '&' ) ||   ( x ==  '\\' ) ||   ( x ==  '(' ) ||   ( x ==  ')' ) ||   ( x ==  '*' ) ||   ( x ==  '+' ) ||   ( x ==  ',' ) ||   ( x ==  ';' ) ||   ( x ==  '=' ) ||   ( x ==  ':' ) ||   ( x ==  '@' )) { /* case */
 
 }
 else  if ( ( x ==  '[' ) ||   ( x ==  ']' )) { /* case */
 
 }
 else  if ( ( x ==  '%' )) { /* case */
 
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
 /* returns 0 */ bool net_dot_urllib__valid_optional_port(string port) {
 
 if (string_eq( port , tos2((byte*)"") ) ) {
 
return  1 ;
 
 }
 ;
 
 if ( string_at( port , 0) != ':' ) {
 
return  0 ;
 
 }
 ;
 
 string tmp166 =  string_right( port , 1 );
 ;
for (int tmp167 = 0; tmp167 < tmp166 .len; tmp167 ++) {
 byte b = ((byte *) tmp166.str)[tmp167];
 
 
 if ( b < '0'  ||  b > '9' ) {
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
return  1 ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__URL_str(net_dot_urllib__URL* u) {
 
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
 /* returns 0 */ Option_net_dot_urllib__Values net_dot_urllib__parse_query(string query) {
 
net_dot_urllib__Values m= net_dot_urllib__new_values ( ) ;
 
Option_bool tmp174 =  net_dot_urllib___parse_query (& /*112 EXP:"net_dot_urllib__Values*" GOT:"net_dot_urllib__Values" */ m , query ) ;
 if (!tmp174 .ok) {
 string err = tmp174 . error;
 
return  v_error ( err ) ;
 
 }
 bool _ = *(bool*) tmp174 . data;
 ;
 
net_dot_urllib__Values tmp175 = OPTION_CAST(net_dot_urllib__Values)( m); return opt_ok(&tmp175, sizeof(net_dot_urllib__Values)) ;
 
 
 }
 /* returns 0 */ net_dot_urllib__Values net_dot_urllib__parse_query_silent(string query) {
 
net_dot_urllib__Values m= net_dot_urllib__new_values ( ) ;
 
Option_bool _= net_dot_urllib___parse_query (& /*112 EXP:"net_dot_urllib__Values*" GOT:"net_dot_urllib__Values" */ m , query ) ;
 
return  m ;
 
 
 }
 /* returns 0 */ Option_bool net_dot_urllib___parse_query(net_dot_urllib__Values* m, string query) {
 
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
 
Option_string tmp183 =  net_dot_urllib__query_unescape ( key ) ;
 if (!tmp183 .ok) {
 string err = tmp183 . error;
 
 had_error  =  1 ;
 
 continue
 ;
 
 }
 string k = *(string*) tmp183 . data;
 ;
 
 key  =  k ;
 
Option_string tmp184 =  net_dot_urllib__query_unescape ( value ) ;
 if (!tmp184 .ok) {
 string err = tmp184 . error;
 
 had_error  =  1 ;
 
 continue
 ;
 
 }
 string v = *(string*) tmp184 . data;
 ;
 
 value  =  v ;
 
 net_dot_urllib__Values_add( m , key , value ) ;
 
 }
 ;
 
 if ( had_error ) {
 
return  v_error ( net_dot_urllib__error_msg ( tos2((byte*)"error parsing query string") , tos2((byte*)"") ) ) ;
 
 }
 ;
 
bool tmp185 = OPTION_CAST(bool)( 1); return opt_ok(&tmp185, sizeof(bool)) ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__Values_encode(net_dot_urllib__Values v) {
 
 if ( v .size == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
strings__Builder buf= strings__new_builder ( 200 ) ;
 
array_string keys=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
 map_Value tmp188 =  v .data ;
 array_string keys_tmp188 = map_keys(& tmp188 ); 
 for (int l = 0; l < keys_tmp188 .len; l++) {
   string k = ((string*)keys_tmp188 .data)[l];
 Value _ = {0}; map_get(tmp188, k, & _);
 
 
_PUSH(& keys , ( k ), tmp189, string) ;
 
 }
 ;
 
 array_string_sort(& /* ? */ keys ) ;
 
 array_string tmp190 =  keys;
 ;
for (int tmp191 = 0; tmp191 < tmp190 .len; tmp191 ++) {
 string k = ((string *) tmp190.data)[tmp191];
 
  
 Value tmp192 = {0}; bool tmp193 = map_get( v .data , k, & tmp192); 

Value vs= tmp192 ;
 
string key_kscaped= net_dot_urllib__query_escape ( k ) ;
 
 array_string tmp196 =  vs .data ;
 ;
for (int _ = 0; _ < tmp196 .len; _ ++) {
 string val = ((string *) tmp196 . data)[_];
 
 
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
 /* returns 0 */ string net_dot_urllib__resolve_path(string base, string ref) {
 
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
 
array_string dst=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
array_string src= string_split( full , tos2((byte*)"/") ) ;
 
 array_string tmp203 =  src ;
 ;
for (int _ = 0; _ < tmp203 .len; _ ++) {
 string elem = ((string *) tmp203 . data)[_];
 
 
 if ( string_eq( elem,  tos2((byte*)".") )) { /* case */
 
 }
 else  if ( string_eq( elem,  tos2((byte*)"..") )) { /* case */
 
 if ( dst .len > 0 ) {
 
 dst  =  array_left( dst , dst .len - 1 ) ;
 
 }
 ;
 
 }
 else  { // default:
 
_PUSH(& dst , ( elem ), tmp204, string) ;
 
 }
 ;
 
 }
 ;
 
string last= ( *(string*) array__get( src , src .len - 1) ) ;
 
 if (string_eq( last , tos2((byte*)".") )  || string_eq( last , tos2((byte*)"..") ) ) {
 
_PUSH(& dst , ( tos2((byte*)"") ), tmp208, string) ;
 
 }
 ;
 
return string_add( tos2((byte*)"/") , string_trim_left( array_string_join( dst , tos2((byte*)"/") ) , tos2((byte*)"/") ) ) ;
 
 
 }
 /* returns 0 */ bool net_dot_urllib__URL_is_abs(net_dot_urllib__URL* u) {
 
return string_ne( u ->scheme , tos2((byte*)"") ) ;
 
 
 }
 /* returns 0 */ Option_net_dot_urllib__URL net_dot_urllib__URL_parse(net_dot_urllib__URL* u, string ref) {
 
Option_net_dot_urllib__URL tmp209 =  net_dot_urllib__parse ( ref ) ;
 if (!tmp209 .ok) {
 string err = tmp209 . error;
 
return  v_error ( err ) ;
 
 }
 net_dot_urllib__URL refurl = *(net_dot_urllib__URL*) tmp209 . data;
 ;
 
Option_net_dot_urllib__URL tmp210 = OPTION_CAST(Option_net_dot_urllib__URL)( net_dot_urllib__URL_resolve_reference(& /* ? */* u ,& /*112 EXP:"net_dot_urllib__URL*" GOT:"net_dot_urllib__URL" */ refurl )); return opt_ok(&tmp210, sizeof(net_dot_urllib__URL)) ;
 
 
 }
 /* returns 0 */ Option_net_dot_urllib__URL net_dot_urllib__URL_resolve_reference(net_dot_urllib__URL* u, net_dot_urllib__URL* ref) {
 
net_dot_urllib__URL url= * ref ;
 
 if (string_eq( ref ->scheme , tos2((byte*)"") ) ) {
 
 url .scheme  =  u ->scheme ;
 
 }
 ;
 
 if (string_ne( ref ->scheme , tos2((byte*)"") )  || string_ne( ref ->host , tos2((byte*)"") )  ||  ! net_dot_urllib__Userinfo_empty(& /* ? */* ref ->user ) ) {
 
 net_dot_urllib__URL_set_path(& /* ? */ url , net_dot_urllib__resolve_path ( net_dot_urllib__URL_escaped_path(& /* ? */* ref ) , tos2((byte*)"") ) ) ;
 
net_dot_urllib__URL tmp212 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp212, sizeof(net_dot_urllib__URL)) ;
 
 }
 ;
 
 if (string_ne( ref ->opaque , tos2((byte*)"") ) ) {
 
 url .user  =  net_dot_urllib__user ( tos2((byte*)"") ) ;
 
 url .host  =  tos2((byte*)"") ;
 
 url .path  =  tos2((byte*)"") ;
 
net_dot_urllib__URL tmp213 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp213, sizeof(net_dot_urllib__URL)) ;
 
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
 
net_dot_urllib__URL tmp214 = OPTION_CAST(net_dot_urllib__URL)( url); return opt_ok(&tmp214, sizeof(net_dot_urllib__URL)) ;
 
 
 }
 /* returns 0 */ net_dot_urllib__Values net_dot_urllib__URL_query(net_dot_urllib__URL* u) {
 
net_dot_urllib__Values v= net_dot_urllib__parse_query_silent ( u ->raw_query ) ;
 
return  v ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__URL_request_uri(net_dot_urllib__URL* u) {
 
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
 /* returns 0 */ string net_dot_urllib__URL_hostname(net_dot_urllib__URL* u) {
 
return  net_dot_urllib__strip_port ( u ->host ) ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__URL_port(net_dot_urllib__URL* u) {
 
return  net_dot_urllib__port_only ( u ->host ) ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__strip_port(string hostport) {
 
int colon= string_index( hostport , tos2((byte*)":") ) ;
 
 if ( colon == - 1 ) {
 
return  hostport ;
 
 }
 ;
 
int i= string_index( hostport , tos2((byte*)"]") ) ;
 
 if ( i != - 1 ) {
 
return  string_trim_left( string_left( hostport , i ) , tos2((byte*)"[") ) ;
 
 }
 ;
 
return  string_left( hostport , colon ) ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__port_only(string hostport) {
 
int colon= string_index( hostport , tos2((byte*)":") ) ;
 
 if ( colon == - 1 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
int i= string_index( hostport , tos2((byte*)"]:") ) ;
 
 if ( i != - 1 ) {
 
return  string_right( hostport , i + ( tos2((byte*)"]:") .len ) ) ;
 
 }
 ;
 
 if ( string_contains( hostport , tos2((byte*)"]") ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_right( hostport , colon + ( tos2((byte*)":") .len ) ) ;
 
 
 }
 /* returns 0 */ bool net_dot_urllib__valid_userinfo(string s) {
 
 string tmp221 =  s;
 ;
for (int tmp222 = 0; tmp222 < tmp221 .len; tmp222 ++) {
 byte r = ((byte *) tmp221.str)[tmp222];
 
 
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
 
 if ( ( r ==  '-' ) ||   ( r ==  '.' ) ||   ( r ==  '_' ) ||   ( r ==  ':' ) ||   ( r ==  '~' ) ||   ( r ==  '!' ) ||   ( r ==  '$' ) ||   ( r ==  '&' ) ||   ( r ==  '\\' ) ||   ( r ==  '(' ) ||   ( r ==  ')' ) ||   ( r ==  '*' ) ||   ( r ==  '+' ) ||   ( r ==  ',' ) ||   ( r ==  ';' ) ||   ( r ==  '=' ) ||   ( r ==  '%' ) ||   ( r ==  '@' )) { /* case */
 
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
 /* returns 0 */ bool net_dot_urllib__string_contains_ctl_byte(string s) {
 
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
 /* returns 0 */ bool net_dot_urllib__ishex(byte c) {
 
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
 /* returns 0 */ byte net_dot_urllib__unhex(byte c) {
 
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
 /* returns 0 */ net_dot_urllib__Values net_dot_urllib__new_values() {
 
return  (net_dot_urllib__Values) { .data =  new_map(1, sizeof(Value)) , .size = 0 } ;
 
 
 }
 /* returns 0 */ array_string Value_all(Value* v) {
 
return  v ->data ;
 
 
 }
 /* returns 0 */ string net_dot_urllib__Values_get(net_dot_urllib__Values v, string key) {
 
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
 /* returns 0 */ array_string net_dot_urllib__Values_get_all(net_dot_urllib__Values v, string key) {
 
 if ( v .data .size == 0 ) {
 
return new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
 }
 ;
  
 Value tmp6 = {0}; bool tmp7 = map_get( v .data , key, & tmp6); 

Value vs= tmp6 ;
 
 if ( vs .data .len == 0 ) {
 
return new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
 }
 ;
 
return  vs .data ;
 
 
 }
 /* returns 0 */ void net_dot_urllib__Values_set(net_dot_urllib__Values* v, string key, string value) {
  
 Value tmp9 = {0}; bool tmp10 = map_get( v ->data , key, & tmp9); 

Value a= tmp9 ;
 
 a .data  = new_array_from_c_array(1, 1, sizeof(string), (string[]) {  value  }) ;
  
 Value tmp12 =  a; 

map__set(& v ->data , key , & tmp12) ;
 
 v ->size  =  v ->data .size ;
 
 
 }
 /* returns 0 */ void net_dot_urllib__Values_add(net_dot_urllib__Values* v, string key, string value) {
  
 Value tmp13 = {0}; bool tmp14 = map_get( v ->data , key, & tmp13); 

Value a= tmp13 ;
 
 if ( a .data .len == 0 ) {
 
 a .data  = new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
 }
 ;
 
_PUSH(& a .data , ( value ), tmp16, string) ;
  
 Value tmp17 =  a; 

map__set(& v ->data , key , & tmp17) ;
 
 v ->size  =  v ->data .size ;
 
 
 }
 /* returns 0 */ void net_dot_urllib__Values_del(net_dot_urllib__Values* v, string key) {
 
 map_delete(& /* ? */ v ->data , key ) ;
 
 v ->size  =  v ->data .size ;
 
 
 }
 /* returns 0 */ void rand__seed(int s) {
 
 srand ( s ) ;
 
 
 }
 /* returns 0 */ int rand__next(int max) {
 
return  rand ( ) % max ;
 
 
 }
 /* returns 0 */ /* returns 0 */ void http__init_module() {
 
 #ifdef __APPLE__
 
 SSL_library_init ( ) ;
 
 #endif
 /* returns 0 */ ;
 
 #ifdef __linux__
 
 SSL_library_init ( ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ http__Response http__ssl_do(int port, string method, string host_name, string path) {
 
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
 
 res  =  BIO_set_conn_hostname ( web ,  addr .str ) ;
 
 if ( res != 1 ) {
 
 }
 ;
  
 struct /*c struct init*/ 

SSL* ssl= 0 ;
 
 BIO_get_ssl ( web ,  & /*vvar*/  ssl ) ;
 
 if ( isnil ( ssl ) ) {
 
 }
 ;
 
string preferred_ciphers= tos2((byte*)"HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4") ;
 
 res  =  SSL_set_cipher_list ( ssl ,  preferred_ciphers .str ) ;
 
 if ( res != 1 ) {
 
 }
 ;
 
 res  =  SSL_set_tlsext_host_name ( ssl ,  host_name .str ) ;
 
 res  =  BIO_do_connect ( web ) ;
 
 res  =  BIO_do_handshake ( web ) ;
 
void* cert= SSL_get_peer_certificate ( ssl ) ;
 
 res  =  SSL_get_verify_result ( ssl ) ;
 
string s= http__build_request_headers ( tos2((byte*)"") , method , host_name , path ) ;
 
 BIO_puts ( web ,  s .str ) ;
 
strings__Builder sb= strings__new_builder ( 100 ) ;
 
 while (1) { 
 
byte buff  [1536 ]= STRUCT_DEFAULT_VALUE ;
 
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
 /* returns 0 */ void http__download_cb(void* ptr, size_t size, size_t nmemb, void* userp) {
 
 
 }
 /* returns 0 */ void http__download_file_with_progress(string url, string out, downloadfn cb, void (*cb_finished)(  /*FFF*/ )) {
 
 
 }
 /* returns 0 */ void http__download_file(string url, string out) {
 
 
 }
 /* returns 0 */ void http__empty() {
 
 
 }
 /* returns 0 */ Option_http__Response http__get(string url) {
 
Option_http__Request tmp1 =  http__new_request ( tos2((byte*)"GET") , url , tos2((byte*)"") ) ;
 if (!tmp1 .ok) {
 string err = tmp1 . error;
 
return  v_error ( err ) ;
 
 }
 http__Request req = *(http__Request*) tmp1 . data;
 ;
 
Option_http__Response tmp2 = OPTION_CAST(Option_http__Response)( http__Request_do(& /* ? */ req )); return opt_ok(&tmp2, sizeof(http__Response)) ;
 
 
 }
 /* returns 0 */ Option_http__Response http__post(string url, string data) {
 
Option_http__Request tmp3 =  http__new_request ( tos2((byte*)"POST") , url , data ) ;
 if (!tmp3 .ok) {
 string err = tmp3 . error;
 
return  v_error ( err ) ;
 
 }
 http__Request req = *(http__Request*) tmp3 . data;
 ;
 
Option_http__Response tmp4 = OPTION_CAST(Option_http__Response)( http__Request_do(& /* ? */ req )); return opt_ok(&tmp4, sizeof(http__Response)) ;
 
 
 }
 /* returns 0 */ Option_http__Request http__new_request(string typ, string _url, string _data) {
 
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
 
http__Request tmp7 = OPTION_CAST(http__Request)( (http__Request) { .typ =  typ , .url =  url , .data =  data , .ws_func =  0 , .user_ptr =  0 , .headers =  new_map(1, sizeof(string)) , .headers2 = new_array(0, 1, sizeof( string )) , .method = tos("", 0) , .h = tos("", 0) , .cmd = tos("", 0) , .verbose = 0 }); return opt_ok(&tmp7, sizeof(http__Request)) ;
 
 
 }
 /* returns 0 */ string http__get_text(string url) {
 
Option_http__Response tmp8 =  http__get ( url ) ;
 if (!tmp8 .ok) {
 string err = tmp8 . error;
 
return  tos2((byte*)"") ;
 
 }
 http__Response resp = *(http__Response*) tmp8 . data;
 ;
 
return  resp .text ;
 
 
 }
 /* returns 0 */ void http__Request_free(http__Request* req) {
 
 v_map_free( req ->headers ) ;
 
 
 }
 /* returns 0 */ void http__Response_free(http__Response* resp) {
 
 v_map_free( resp ->headers ) ;
 
 
 }
 /* returns 0 */ void http__Request_add_header(http__Request* req, string key, string val) {
  
 string tmp9 =  val; 

map__set(& req ->headers , key , & tmp9) ;
 
 
 }
 /* returns 0 */ Option_http__Response http__Request_do(http__Request* req) {
 
 if (string_eq( req ->typ , tos2((byte*)"POST") ) ) {
 
 }
 ;
 
 map_string tmp10 =  req ->headers ;
 array_string keys_tmp10 = map_keys(& tmp10 ); 
 for (int l = 0; l < keys_tmp10 .len; l++) {
   string key = ((string*)keys_tmp10 .data)[l];
 string val = {0}; map_get(tmp10, key, & val);
 
 
 }
 ;
 
Option_net_dot_urllib__URL tmp11 =  net_dot_urllib__parse ( req ->url ) ;
 if (!tmp11 .ok) {
 string err = tmp11 . error;
 
return  v_error ( _STR("http.request.do: invalid URL %.*s", req ->url.len, req ->url.str) ) ;
 
 }
 net_dot_urllib__URL url = *(net_dot_urllib__URL*) tmp11 . data;
 ;
 
net_dot_urllib__URL rurl= url ;
 
http__Response resp= (http__Response) { .text = tos("", 0) , .headers = new_map(1, sizeof( string )) , .status_code = 0 } ;
 
int no_redirects= 0 ;
 
 while (1) { 
 
 if ( no_redirects == http__max_redirects ) {
 
return  v_error ( _STR("http.request.do: maximum number of redirects reached (%d)", http__max_redirects) ) ;
 
 }
 ;
 
Option_http__Response tmp15 =  http__method_and_url_to_response ( req ->typ , rurl ) ;
 if (!tmp15 .ok) {
 string err = tmp15 . error;
 
return  v_error ( err ) ;
 
 }
 http__Response qresp = *(http__Response*) tmp15 . data;
 ;
 
 resp  =  qresp ;
 
 if ( ! (_IN(int,  resp .status_code , new_array_from_c_array(5, 5, sizeof(int), (int[]) {  301 ,  302 ,  303 ,  307 ,  308  }) ) ) ) {
 
 break
 ;
 
 }
 ;
  
 string tmp16 = tos("", 0); bool tmp17 = map_get( resp .headers , tos2((byte*)"Location"), & tmp16); 
 
 if (!tmp17) tmp16 = tos("", 0); 

string redirect_url= tmp16 ;
 
Option_net_dot_urllib__URL tmp19 =  net_dot_urllib__parse ( redirect_url ) ;
 if (!tmp19 .ok) {
 string err = tmp19 . error;
 
return  v_error ( _STR("http.request.do: invalid URL in redirect %.*s", redirect_url.len, redirect_url.str) ) ;
 
 }
 net_dot_urllib__URL qrurl = *(net_dot_urllib__URL*) tmp19 . data;
 ;
 
 rurl  =  qrurl ;
 
 no_redirects ++ ;
 
 }
 ;
 
http__Response tmp20 = OPTION_CAST(http__Response)( resp); return opt_ok(&tmp20, sizeof(http__Response)) ;
 
 
 }
 /* returns 0 */ Option_http__Response http__method_and_url_to_response(string method, net_dot_urllib__URL url) {
 
string host_name= net_dot_urllib__URL_hostname(& /* ? */ url ) ;
 
string scheme= url .scheme ;
 
string p= string_trim_left( url .path , tos2((byte*)"/") ) ;
 
string path= ( net_dot_urllib__URL_query(& /* ? */ url ) .size > 0 ) ? ( _STR("/%.*s?%.*s", p.len, p.str, net_dot_urllib__Values_encode( net_dot_urllib__URL_query(& /* ? */ url ) ).len, net_dot_urllib__Values_encode( net_dot_urllib__URL_query(& /* ? */ url ) ).str) ) : ( _STR("/%.*s", p.len, p.str) ) ;
 
int nport= string_int( net_dot_urllib__URL_port(& /* ? */ url ) ) ;
 
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
 
http__Response tmp26 = OPTION_CAST(http__Response)( http__ssl_do ( nport , method , host_name , path )); return opt_ok(&tmp26, sizeof(http__Response)) ;
 
 }
  else  if (string_eq( scheme , tos2((byte*)"http") ) ) {
 
Option_http__Response tmp27 = OPTION_CAST(Option_http__Response)( http__http_do ( nport , method , host_name , path )); return opt_ok(&tmp27, sizeof(http__Response)) ;
 
 }
 ;
 
return  v_error ( _STR("http.request.do: unsupported scheme: %.*s", scheme.len, scheme.str) ) ;
 
 
 }
 /* returns 0 */ http__Response http__parse_response(string resp) {
 
map_string headers= new_map(1, sizeof(string)) ;
 
string first_header= string_all_before( resp , tos2((byte*)"\n") ) ;
 
int status_code= 0 ;
 
 if ( string_contains( first_header , tos2((byte*)"HTTP/") ) ) {
 
string val= string_find_between( first_header , tos2((byte*)" ") , tos2((byte*)" ") ) ;
 
 status_code  =  string_int( val ) ;
 
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
  
 string tmp40 =  string_trim_space( val ); 

map__set(& headers , key , & tmp40) ;
 
 }
 ;
  
 string tmp41 = tos("", 0); bool tmp42 = map_get( headers , tos2((byte*)"Transfer-Encoding"), & tmp41); 
 
 if (!tmp42) tmp41 = tos("", 0); 

 if (string_eq( tmp41 , tos2((byte*)"chunked") ) ) {
 
 text  =  http_dot_chunked__decode ( text ) ;
 
 }
 ;
 
return  (http__Response) { .status_code =  status_code , .headers =  headers , .text =  text } ;
 
 
 }
 /* returns 0 */ string http__build_request_headers(string user_agent, string method, string host_name, string path) {
 
string ua= (string_eq( user_agent , tos2((byte*)"") ) ) ? ( tos2((byte*)"v") ) : ( user_agent ) ;
 
return string_add(string_add(string_add( _STR("%.*s %.*s HTTP/1.1\r\n", method.len, method.str, path.len, path.str) , _STR("Host: %.*s\r\n", host_name.len, host_name.str) ) , _STR("User-Agent: %.*s\r\n", ua.len, ua.str) ) , tos2((byte*)"Connection: close\r\n\r\n") ) ;
 
 
 }
 /* returns 0 */ string http__unescape_url(string s) {
 
 v_panic ( tos2((byte*)"http.unescape_url() was replaced with urllib.query_unescape()") ) ;
 
 
 }
 /* returns 0 */ string http__escape_url(string s) {
 
 v_panic ( tos2((byte*)"http.escape_url() was replaced with urllib.query_escape()") ) ;
 
 
 }
 /* returns 0 */ string http__unescape(string s) {
 
 v_panic ( tos2((byte*)"http.unescape() was replaced with http.unescape_url()") ) ;
 
 
 }
 /* returns 0 */ string http__escape(string s) {
 
 v_panic ( tos2((byte*)"http.escape() was replaced with http.escape_url()") ) ;
 
 
 }
 /* returns 0 */ Option_http__Response http__http_do(int port, string method, string host_name, string path) {
 
int bufsize= 512 ;
 
byte rbuffer  [512 ]= STRUCT_DEFAULT_VALUE ;
 
strings__Builder sb= strings__new_builder ( 100 ) ;
 
string s= http__build_request_headers ( tos2((byte*)"") , method , host_name , path ) ;
 
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
 /* returns 0 */ int http_dot_chunked__ChunkScanner_read_chunk_size(http_dot_chunked__ChunkScanner* s) {
 
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
 /* returns 0 */ byte http_dot_chunked__unhex(byte c) {
 
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
 /* returns 0 */ void http_dot_chunked__ChunkScanner_skip_crlf(http_dot_chunked__ChunkScanner* s) {
 
 s ->pos  +=  2 ;
 
 
 }
 /* returns 0 */ string http_dot_chunked__ChunkScanner_read_chunk(http_dot_chunked__ChunkScanner* s, int chunksize) {
 
int startpos= s ->pos ;
 
 s ->pos  +=  chunksize ;
 
return  string_substr( s ->text , startpos , s ->pos ) ;
 
 
 }
 /* returns 0 */ string http_dot_chunked__decode(string text) {
 
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
 /* returns 0 */ /* returns 0 */ void time__remove_me_when_c_bug_is_fixed() {
 
 
 }
 /* returns 0 */ time__Time time__now() {
 
void* t= time ( 0 ) ;
  
 struct /*c struct init*/ 

tm* now= 0 ;
 
 now  =  localtime ( & /*vvar*/  t ) ;
 
return  time__convert_ctime (* now ) ;
 
 
 }
 /* returns 0 */ time__Time time__random() {
 
int now_unix= time__now ( ) .uni ;
 
int rand_unix= rand__next ( now_unix ) ;
 
return  time__unix ( rand_unix ) ;
 
 
 }
 /* returns 0 */ time__Time time__unix(int abs) {
 
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
 
return  (struct time__Time) { .year =  year , .month =  2 , .day =  day , .hour =  hour , .minute =  minute , .second =  second , .uni = 0 } ;
 
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
 
return  (time__Time) { .year =  year , .month =  month , .day =  day , .hour =  hour , .minute =  minute , .second =  second , .uni = 0 } ;
 
 
 }
 /* returns 0 */ time__Time time__convert_ctime(struct /*TM*/ tm t) {
 
return  (time__Time) { .year =  t .tm_year + 1900 , .month =  t .tm_mon + 1 , .day =  t .tm_mday , .hour =  t .tm_hour , .minute =  t .tm_min , .second =  t .tm_sec , .uni =  mktime ( & /*vvar*/  t ) } ;
 
 
 }
 /* returns 0 */ string time__Time_format_ss(time__Time t) {
 
return  _STR("%d-%02d-%02d %02d:%02d:%02d", t .year, t .month, t .day, t .hour, t .minute, t .second) ;
 
 
 }
 /* returns 0 */ string time__Time_format(time__Time t) {
 
return  _STR("%d-%02d-%02d %02d:%02d", t .year, t .month, t .day, t .hour, t .minute) ;
 
 
 }
 /* returns 0 */ string time__Time_smonth(time__Time t) {
 
int i= t .month - 1 ;
 
return  string_substr( time__Months , i * 3 , ( i + 1 ) * 3 ) ;
 
 
 }
 /* returns 0 */ string time__Time_hhmm(time__Time t) {
 
return  _STR("%02d:%02d", t .hour, t .minute) ;
 
 
 }
 /* returns 0 */ string time__Time_hhmm12(time__Time t) {
 
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
 /* returns 0 */ string time__Time_hhmmss(time__Time t) {
 
return  _STR("%02d:%02d:%02d", t .hour, t .minute, t .second) ;
 
 
 }
 /* returns 0 */ string time__Time_ymmdd(time__Time t) {
 
return  _STR("%d-%02d-%02d", t .year, t .month, t .day) ;
 
 
 }
 /* returns 0 */ string time__Time_md(time__Time t) {
 
string s= _STR("%.*s %d", time__Time_smonth( t ).len, time__Time_smonth( t ).str, t .day) ;
 
return  s ;
 
 
 }
 /* returns 0 */ string time__Time_clean(time__Time t) {
 
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
 /* returns 0 */ string time__Time_clean12(time__Time t) {
 
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
 /* returns 0 */ time__Time time__parse(string s) {
 
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
 
return  time__new_time ( (time__Time) { .year =  string_int( ( *(string*) array__get( ymd , 0) ) ) , .month =  string_int( ( *(string*) array__get( ymd , 1) ) ) , .day =  string_int( ( *(string*) array__get( ymd , 2) ) ) , .hour =  string_int( hour ) , .minute =  string_int( minute ) , .second =  string_int( second ) , .uni = 0 } ) ;
 
 
 }
 /* returns 0 */ time__Time time__new_time(time__Time t) {
 
return  (time__Time){ .uni =  time__Time_calc_unix(& /* ? */ t ) , .year = t . year, .month = t . month, .day = t . day, .hour = t . hour, .minute = t . minute, .second = t . second, } ;
 
 
 }
 /* returns 0 */ int time__Time_calc_unix(time__Time* t) {
 
 if ( t ->uni != 0 ) {
 
return  t ->uni ;
 
 }


struct /*TM*/ tm tt= (struct tm) { .tm_sec =  t ->second , .tm_min =  t ->minute , .tm_hour =  t ->hour , .tm_mday =  t ->day , .tm_mon =  t ->month - 1 , .tm_year =  t ->year - 1900 } ;
 
return  mktime ( & /*vvar*/  tt ) ;
 
 
 }
 /* returns 0 */ time__Time time__Time_add_seconds(time__Time t, int seconds) {
 
return  time__unix ( t .uni + seconds ) ;
 
 
 }
 /* returns 0 */ int time__since(time__Time t) {
 
return  0 ;
 
 
 }
 /* returns 0 */ string time__Time_relative(time__Time t) {
 
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
 /* returns 0 */ int time__day_of_week(int y, int m, int d) {
 
return  0 ;
 
 
 }
 /* returns 0 */ int time__Time_day_of_week(time__Time t) {
 
return  time__day_of_week ( t .year , t .month , t .day ) ;
 
 
 }
 /* returns 0 */ string time__Time_weekday_str(time__Time t) {
 
int i= time__Time_day_of_week( t ) - 1 ;
 
return  string_substr( time__Days , i * 3 , ( i + 1 ) * 3 ) ;
 
 
 }
 /* returns 0 */ i64 time__ticks() {
 
 #ifdef _WIN32
 
return  GetTickCount ( ) ;
 
 /* returns 0 */ #else
  
 struct /*c struct init*/ 

timeval ts= (struct timeval) { .tv_sec = 0 , .tv_usec = 0 } ;
 
 gettimeofday ( & /*vvar*/  ts ,  0 ) ;
 
return  ts .tv_sec * 1000 + ( ts .tv_usec / 1000 ) ;
 
 #endif
 /* returns 1 */ ;
 
 
 }
 /* returns 0 */ void time__sleep(int seconds) {
 
 #ifdef _WIN32
 
 _sleep ( seconds * 1000 ) ;
 
 /* returns 0 */ #else
 
 sleep ( seconds ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ void time__usleep(int n) {
 
 #ifdef _WIN32
 
 /* returns 0 */ #else
 
 usleep ( n ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ void time__sleep_ms(int n) {
 
 #ifdef _WIN32
 
 Sleep ( n ) ;
 
 /* returns 0 */ #else
 
 usleep ( n * 1000 ) ;
 
 #endif
 /* returns 0 */ ;
 
 
 }
 /* returns 0 */ bool time__is_leap_year(int year) {
 
return  ( year % 4 == 0 )  &&  ( year % 100 != 0  ||  year % 400 == 0 ) ;
 
 
 }
 /* returns 0 */ Option_int time__days_in_month(int month, int year) {
 
 if ( month > 12  ||  month < 1 ) {
 
return  v_error ( _STR("Invalid month: %d", month) ) ;
 
 }
 ;
 
int extra= ( month == 2  &&  time__is_leap_year ( year ) ) ? ( 1 ) : ( 0 ) ;
 
int res= ( *(int*) array__get( time__MonthDays , month - 1) ) + extra ;
 
int tmp56 = OPTION_CAST(int)( res); return opt_ok(&tmp56, sizeof(int)) ;
 
 
 }
 /* returns 0 */ /* returns 0 */ /* returns 0 */ /* returns 0 */ /* returns 0 */ pg__DB pg__connect(pg__Config config) {
 
string conninfo= _STR("host=%.*s user=%.*s dbname=%.*s", config .host.len, config .host.str, config .user.len, config .user.str, config .dbname.len, config .dbname.str) ;
 
PGconn* conn= PQconnectdb ( conninfo .str ) ;
 
int status= PQstatus ( conn ) ;
 
 if ( status != CONNECTION_OK ) {
 
byte* error_msg= PQerrorMessage ( conn ) ;
 
 eprintln (string_add( tos2((byte*)"Connection to a PG database failed: ") , (tos2( error_msg ) ) ) ) ;
 
 v_exit ( 1 ) ;
 
 }
 ;
 
return  (pg__DB) { .conn =  conn } ;
 
 
 }
 /* returns 0 */ array_pg__Row pg__res_to_rows(void* res) {
 
void* nr_rows= PQntuples ( res ) ;
 
void* nr_cols= PQnfields ( res ) ;
 
array_pg__Row rows=new_array_from_c_array(0, 0, sizeof(pg__Row), (pg__Row[]) {EMPTY_STRUCT_INIT   }) ;
 
 for (
int i= 0  ;  i < nr_rows  ;  i ++ ) { 
 
 
pg__Row row= (pg__Row) { .vals = new_array(0, 1, sizeof( string )) } ;
 
 for (
int j= 0  ;  j < nr_cols  ;  j ++ ) { 
 
 
byte* val= PQgetvalue ( res ,  i ,  j ) ;
 
_PUSH(& row .vals , ( (tos2( val ) ) ), tmp12, string) ;
 
 }
 ;
 
_PUSH(& rows , ( row ), tmp13, pg__Row) ;
 
 }
 ;
 
return  rows ;
 
 
 }
 /* returns 0 */ int pg__DB_q_int(pg__DB db, string query) {
 
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
 
return  string_int( val ) ;
 
 
 }
 /* returns 0 */ string pg__DB_q_string(pg__DB db, string query) {
 
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
 /* returns 0 */ array_pg__Row pg__DB_q_strings(pg__DB db, string query) {
 
return  pg__DB_exec( db , query ) ;
 
 
 }
 /* returns 0 */ array_pg__Row pg__DB_exec(pg__DB db, string query) {
 
void* res= PQexec ( db .conn ,  query .str ) ;
 
string e= (tos2( PQerrorMessage ( db .conn ) ) ) ;
 
 if (string_ne( e , tos2((byte*)"") ) ) {
 
 println ( tos2((byte*)"pg exec error:") ) ;
 
 println ( e ) ;
 
return  pg__res_to_rows ( res ) ;
 
 }
 ;
 
return  pg__res_to_rows ( res ) ;
 
 
 }
 /* returns 0 */ Option_pg__Row pg__rows_first_or_empty(array_pg__Row rows) {
 
 if ( rows .len == 0 ) {
 
return  v_error ( tos2((byte*)"no row") ) ;
 
 }
 ;
 
pg__Row tmp32 = OPTION_CAST(pg__Row)( ( *(pg__Row*) array__get( rows , 0) )); return opt_ok(&tmp32, sizeof(pg__Row)) ;
 
 
 }
 /* returns 0 */ Option_pg__Row pg__DB_exec_one(pg__DB db, string query) {
 
void* res= PQexec ( db .conn ,  query .str ) ;
 
string e= (tos2( PQerrorMessage ( db .conn ) ) ) ;
 
 if (string_ne( e , tos2((byte*)"") ) ) {
 
return  v_error ( _STR("pg exec error: \"%.*s\"", e.len, e.str) ) ;
 
 }
 ;
 
Option_pg__Row row= pg__rows_first_or_empty ( pg__res_to_rows ( res ) ) ;
 
Option_pg__Row tmp36 = OPTION_CAST(Option_pg__Row)( row); return opt_ok(&tmp36, sizeof(pg__Row)) ;
 
 
 }
 /* returns 0 */ array_pg__Row pg__DB_exec_param2(pg__DB db, string query, string param, string param2) {
 
byteptr param_vals  [2 ]= STRUCT_DEFAULT_VALUE ;
 
 param_vals [ 0 ]/*rbyteptr 1*/  =  param .str ;
 
 param_vals [ 1 ]/*rbyteptr 1*/  =  param2 .str ;
 
void* res= PQexecParams ( db .conn ,  query .str ,  2 ,  0 ,  param_vals ,  0 ,  0 ,  0 ) ;
 
string e= (tos2( PQerrorMessage ( db .conn ) ) ) ;
 
 if (string_ne( e , tos2((byte*)"") ) ) {
 
 println ( tos2((byte*)"pg exec2 error:") ) ;
 
 println ( e ) ;
 
return  pg__res_to_rows ( res ) ;
 
 }
 ;
 
return  pg__res_to_rows ( res ) ;
 
 
 }
 /* returns 0 */ array_pg__Row pg__DB_exec_param(pg__DB db, string query, string param) {
 
byteptr param_vals  [1 ]= STRUCT_DEFAULT_VALUE ;
 
 param_vals [ 0 ]/*rbyteptr 1*/  =  param .str ;
 
void* res= PQexecParams ( db .conn ,  query .str ,  1 ,  0 ,  param_vals ,  0 ,  0 ,  0 ) ;
 
return  pg__res_to_rows ( res ) ;
 
 
 }
 /* returns 0 */ void vweb__Context_text(vweb__Context ctx, string s) {
 
string h= array_string_join( ctx .headers , tos2((byte*)"\n") ) ;
 
 net__Socket_write( ctx .conn , _STR("HTTP/1.1 200 OK\nContent-Type: text/plain\n%.*s\n%.*s", h.len, h.str, s.len, s.str) ) ;
 
 
 }
 /* returns 0 */ void vweb__Context_json(vweb__Context ctx, string s) {
 
string h= array_string_join( ctx .headers , tos2((byte*)"\n") ) ;
 
 net__Socket_write( ctx .conn , _STR("HTTP/1.1 200 OK\nContent-Type: application/json\n%.*s\n%.*s", h.len, h.str, s.len, s.str) ) ;
 
 
 }
 /* returns 0 */ void vweb__Context_redirect(vweb__Context ctx, string url) {
 
string h= array_string_join( ctx .headers , tos2((byte*)"\n") ) ;
 
 net__Socket_write( ctx .conn , _STR("HTTP/1.1 302 Found\nLocation: %.*s\n%.*s", url.len, url.str, h.len, h.str) ) ;
 
 
 }
 /* returns 0 */ void vweb__Context_not_found(vweb__Context ctx, string s) {
 
 net__Socket_write( ctx .conn , tos2((byte*)"HTTP/1.1 404 Not Found") ) ;
 
 
 }
 /* returns 0 */ void vweb__Context_set_cookie(vweb__Context* ctx, string key, string val) {
 
 vweb__Context_set_header( ctx , tos2((byte*)"Set-Cookie") , _STR("%.*s=%.*s", key.len, key.str, val.len, val.str) ) ;
 
 
 }
 /* returns 0 */ string vweb__Context_get_cookie(vweb__Context ctx, string key) {
 
 array_string tmp4 =  ctx .req .headers2;
 ;
for (int tmp5 = 0; tmp5 < tmp4 .len; tmp5 ++) {
 string h = ((string *) tmp4.data)[tmp5];
 
 
 if ( string_starts_with( h , tos2((byte*)"Cookie:") )  ||  string_starts_with( h , tos2((byte*)"cookie:") ) ) {
 
string cookie= string_right( h , 7 ) ;
 
return  string_find_between( cookie , _STR(" %.*s=", key.len, key.str) , tos2((byte*)";") ) ;
 
 }
 ;
 
 }
 ;
 
return  tos2((byte*)"") ;
 
 
 }
 /* returns 0 */ void vweb__Context_set_header(vweb__Context* ctx, string key, string val) {
 
_PUSH(& ctx ->headers , ( _STR("%.*s: %.*s", key.len, key.str, val.len, val.str) ), tmp7, string) ;
 
 
 }
 /* returns 0 */ void vweb__Context_html(vweb__Context ctx, string html) {
 
string h= array_string_join( ctx .headers , tos2((byte*)"\n") ) ;
 
 net__Socket_write( ctx .conn , _STR("HTTP/1.1 200 OK\nContent-Type: text/html\n%.*s\n\n%.*s", h.len, h.str, html.len, html.str) ) ;
 
 
 }
 /* returns 0 */ void vweb__run_App(int port) {
 // T start 2 println
 
printf( "Running vweb app on http://localhost:%d ...\n", port ) ;
 
Option_net__Socket tmp9 =  net__listen ( port ) ;
 if (!tmp9 .ok) {
 string err = tmp9 . error;
 
 v_panic ( tos2((byte*)"failed to listen") ) ;
 
 }
 net__Socket l = *(net__Socket*) tmp9 . data;
 ;
 
App app= (App) { } ;
 
 App_init(& /* ? */ app ) ;
 
 while (1) { 
 
Option_net__Socket tmp11 =  net__Socket_accept( l ) ;
 if (!tmp11 .ok) {
 string err = tmp11 . error;
 
 v_panic ( tos2((byte*)"accept() failed") ) ;
 
 }
 net__Socket conn = *(net__Socket*) tmp11 . data;
 ;
 
string s= net__Socket_read_line( conn ) ;
 
 if (string_eq( s , tos2((byte*)"") ) ) {
 
 net__Socket_write( conn , tos2((byte*)"HTTP/1.1 500 Not Found \nContent-Type: text/plain \n\n500") ) ;
 
 net__Socket_close( conn ) ;
 
 continue
 ;
 
 }
 ;
 
array_string lines= string_split_into_lines( s ) ;
 
array_string headers=new_array_from_c_array(0, 0, sizeof(string), (string[]) {EMPTY_STRUCT_INIT   }) ;
 
 array_string tmp15 =  lines ;
 ;
for (int i = 0; i < tmp15 .len; i ++) {
 string line = ((string *) tmp15 . data)[i];
 
 
 if ( i == 0 ) {
 
 continue
 ;
 
 }
 ;
 
array_string words= string_split( line , tos2((byte*)":") ) ;
 
 if ( words .len != 2 ) {
 
 continue
 ;
 
 }
 ;
 
_PUSH(& headers , ( line ), tmp17, string) ;
 
 }
 ;
 
string first_line= string_all_before( s , tos2((byte*)"\n") ) ;
 
array_string vals= string_split( first_line , tos2((byte*)" ") ) ;
 
string action= string_all_before( string_right( ( *(string*) array__get( vals , 1) ) , 1 ) , tos2((byte*)"/") ) ;
 
 if ( string_contains( action , tos2((byte*)"?") ) ) {
 
 action  =  string_all_before( action , tos2((byte*)"?") ) ;
 
 }
 ;
 
 if (string_eq( action , tos2((byte*)"") ) ) {
 
 action  =  tos2((byte*)"index") ;
 
 }
 ;
 
http__Request req= (http__Request) { .headers =  new_map(1, sizeof(string)) , .headers2 =  headers , .ws_func =  0 , .user_ptr =  0 , .method =  ( *(string*) array__get( vals , 0) ) , .url =  ( *(string*) array__get( vals , 1) ) , .h = tos("", 0) , .cmd = tos("", 0) , .typ = tos("", 0) , .data = tos("", 0) , .verbose = 0 } ;
 
 #ifdef VDEBUG
 
printf( "vweb action = \"%.*s\"\n", action.len, action.str ) ;
 
 #endif
 /* returns 0 */ ;
 
 app .vweb  =  (vweb__Context) { .req =  req , .conn =  conn , .form =  new_map(1, sizeof(string)) , .static_files =  new_map(1, sizeof(string)) , .static_mime_types =  new_map(1, sizeof(string)) , .headers = new_array(0, 1, sizeof( string )) } ;
 
 if (_IN(string,  req .method ,  vweb__methods_with_form ) ) {
 
 vweb__Context_parse_form(& /* ? */ app .vweb , s ) ;
 
 }
 ;
 
 if ( vals .len < 2 ) {
 
 #ifdef VDEBUG
 
 println ( tos2((byte*)"no vals for http") ) ;
 
 #endif
 /* returns 0 */ ;
 
 net__Socket_close( conn ) ;
 
 continue
 ;
 
 }
 ;
 
 if ( string_eq(action, _STR("init")) ) App_init(& app); else {
 
 net__Socket_write( conn , tos2((byte*)"HTTP/1.1 404 Not Found \nContent-Type: text/plain \n\n404 not found") ) ;
 
 }
 ;
 
 net__Socket_close( conn ) ;
 
 }
 ;
 
 }
 
 /* returns 0 */ 
 /* returns 0 */ void vweb__Context_parse_form(vweb__Context* ctx, string s) {
 
 if ( ! (_IN(string,  ctx ->req .method ,  vweb__methods_with_form ) ) ) {
 
 return ;
 
 }
 ;
 
int pos= string_index( s , tos2((byte*)"\r\n\r\n") ) ;
 
 if ( pos > - 1 ) {
 
string str_form= string_substr( s , pos , s .len ) ;
 
 str_form  =  string_replace( str_form , tos2((byte*)"+") , tos2((byte*)" ") ) ;
 
array_string words= string_split( str_form , tos2((byte*)"&") ) ;
 
 array_string tmp50 =  words;
 ;
for (int tmp51 = 0; tmp51 < tmp50 .len; tmp51 ++) {
 string word = ((string *) tmp50.data)[tmp51];
 
 
 #ifdef VDEBUG
 
printf( "parse form keyval=\"%.*s\"\n", word.len, word.str ) ;
 
 #endif
 /* returns 0 */ ;
 
array_string keyval= string_split( string_trim_space( word ) , tos2((byte*)"=") ) ;
 
 if ( keyval .len != 2 ) {
 
 continue
 ;
 
 }
 ;
 
string key= ( *(string*) array__get( keyval , 0) ) ;
 
Option_string tmp58 =  net_dot_urllib__query_unescape ( ( *(string*) array__get( keyval , 1) ) ) ;
 if (!tmp58 .ok) {
 string err = tmp58 . error;
 
 continue
 ;
 
 }
 string val = *(string*) tmp58 . data;
 ;
 
 #ifdef VDEBUG
 
printf( "http form \"%.*s\" => \"%.*s\"\n", key.len, key.str, val.len, val.str ) ;
 
 #endif
 /* returns 0 */ ;
  
 string tmp59 =  val; 

map__set(& ctx ->form , key , & tmp59) ;
 
 }
 ;
 
 }
 ;
 
 
 }
 /* returns 0 */ void vweb__Context_scan_static_directory(vweb__Context* ctx, string directory_path, string mount_path) {
 
array_string files= os__ls ( directory_path ) ;
 
 if ( files .len > 0 ) {
 
 array_string tmp61 =  files;
 ;
for (int tmp62 = 0; tmp62 < tmp61 .len; tmp62 ++) {
 string file = ((string *) tmp61.data)[tmp62];
 
 
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
  
 string tmp66 = string_add(string_add( directory_path , tos2((byte*)"/") ) , file ); 

map__set(& ctx ->static_files ,string_add(string_add( mount_path , tos2((byte*)"/") ) , file ) , & tmp66) ;
  
 string tmp67 = tos("", 0); bool tmp68 = map_get( vweb__mime_types , ext, & tmp67); 
 
 if (!tmp68) tmp67 = tos("", 0); 
 
 string tmp69 =  tmp67; 

map__set(& ctx ->static_mime_types ,string_add(string_add( mount_path , tos2((byte*)"/") ) , file ) , & tmp69) ;
 
 }
 ;
 
 }
 ;
 
 }
 ;
 
 
 }
 /* returns 0 */ bool vweb__Context_handle_static(vweb__Context* ctx, string directory_path) {
 
 vweb__Context_scan_static_directory( ctx , directory_path , tos2((byte*)"") ) ;
  
 string tmp70 = tos("", 0); bool tmp71 = map_get( ctx ->static_files , ctx ->req .url, & tmp70); 
 
 if (!tmp71) tmp70 = tos("", 0); 

string static_file= tmp70 ;
  
 string tmp73 = tos("", 0); bool tmp74 = map_get( ctx ->static_mime_types , ctx ->req .url, & tmp73); 
 
 if (!tmp74) tmp73 = tos("", 0); 

string mime_type= tmp73 ;
 
 if (string_ne( static_file , tos2((byte*)"") ) ) {
 
Option_string tmp76 =  os__read_file ( static_file ) ;
 if (!tmp76 .ok) {
 string err = tmp76 . error;
 
return  0 ;
 
 }
 string data = *(string*) tmp76 . data;
 ;
 
 net__Socket_write( ctx ->conn , _STR("HTTP/1.1 200 OK\nContent-Type: %.*s\n\n%.*s", mime_type.len, mime_type.str, data.len, data.str) ) ;
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 
 }
 /* returns 0 */ void vweb__Context_serve_static(vweb__Context* ctx, string url, string file_path, string mime_type) {
  
 string tmp77 =  file_path; 

map__set(& ctx ->static_files , url , & tmp77) ;
  
 string tmp78 =  mime_type; 

map__set(& ctx ->static_mime_types , url , & tmp78) ;
 
 
 }
