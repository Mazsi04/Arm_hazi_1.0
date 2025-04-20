#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0)
{

}

void Model::tick()
{

}

//int Model::getValue() const { return myValue; } // Getter függvény

/*void Model::setValue(int value)
{
    myValue = value;
    if (modelListener)
    {
    	modelListener->onValueUpdated(value); // Értesítjük a nézetet
    }
}*/


//Model model; // Globális példány
