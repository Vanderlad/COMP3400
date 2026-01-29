// euclidean does not override all pure virtual
// functions and so is still an abstract class...
class euclidean : public position
{
public:
  void move(int dx, int dy) override 
  { 
    position::move(dx, dy); 
  }
};
