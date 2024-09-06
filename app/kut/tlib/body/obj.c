Obj *obj_new(char *type, void *value) {
  Obj *this = MALLOC(Obj);
  this->type = type;
  this->value = value;
  return this;
}
