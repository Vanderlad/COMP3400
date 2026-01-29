void foo(int * const v, int const * const k)
{
  *v += *k;
}

int main()
{
  int i=2, j=45;
  foo(&i, &j);
}
