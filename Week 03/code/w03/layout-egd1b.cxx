struct D_mem_layout_wont_compile 
{
  int z; int a; int b; // D::B path
  int z; int a; int y; int c; // D::C path
  int d;
};
