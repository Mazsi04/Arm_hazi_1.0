#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }
    void tick();
   // int getValue () const;  // Getter függvény
   // void setValue(int value); // Setter függvény
    void setListener(ModelListener* listener) { modelListener = listener; }

protected:
    ModelListener* modelListener;
    //int myValue; // Ez tárolja a főprogramban a megjelenítendő értéket
};

//extern Model model; // Külső hivatkozás

#endif // MODEL_HPP
