int foo(int i)
{
  switch (i)
  {
    case 1: 
      [[fallthrough]];

    case 2: [[likely]]
      return 1;
  }
  return 0;
}
