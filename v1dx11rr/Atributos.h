#pragma once
#include <D3DX10.h>


class Atributos
{
public:
    int vida;
    bool vivo;

    Atributos()
    {
        vida = 0;
        vivo = true;
    }
};

class Jugador : Atributos
{

public:

	Jugador()
	{
		vida = 4;
		vivo = true;
	}

};

class Enemigo : Atributos
{
public:
    Enemigo()
    {
        vida = 3;
        vivo = true;
    }
};

class ObjetoInteractivo {
public:
    virtual D3DXVECTOR3 getPosicion() = 0;
    virtual float getRadio() = 0;
    virtual void onCollision() = 0;
};