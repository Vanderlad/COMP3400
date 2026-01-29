class position
{ // abstract class due to pure virtual functions
public:
  virtual ~position() { } // ensure proper destruction
  virtual int getX() const = 0;
  virtual int getY() const = 0;
  virtual void setX(int x) = 0;
  virtual void setY(int y) = 0;
  virtual void move(int dx, int dy) = 0;
};

void position::move(int dx, int dy) { 
  setX(getX()+dx); setY(getY()+dy);
}
