%module PSON
%{
extern void* parse(const char* s);
extern char get_type(void* obj);
extern int get_int(void* obj);
extern float get_float(void* obj);
extern const char* get_str(void* obj);
extern unsigned get_len(void* obj);
extern void* get_at(void* obj, unsigned idx);
extern const char* get_map_next(void* iter);
extern bool get_map_iter_alive(void* obj, void* iter);
extern bool get_map_contains(void* obj, const char* idx);
extern void* get_map_iter(void* iter);
extern void* get_map_at(void* obj, const char* idx);
extern void destroy_iter(void* iter);
extern void destroy(void* obj);
%}
extern void* parse(const char* s);
extern char get_type(void* obj);
extern int get_int(void* obj);
extern float get_float(void* obj);
extern const char* get_str(void* obj);
extern unsigned get_len(void* obj);
extern void* get_at(void* obj, unsigned idx);
extern const char* get_map_next(void* iter);
extern bool get_map_iter_alive(void* obj, void* iter);
extern bool get_map_contains(void* obj, const char* idx);
extern void* get_map_iter(void* iter);
extern void* get_map_at(void* obj, const char* idx);
extern void destroy_iter(void* iter);
extern void destroy(void* obj);
