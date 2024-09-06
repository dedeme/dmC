typedef struct obj_Obj {
  char *type;
  void *value;
} Obj;
Obj *obj_new(char *type, void *value);
