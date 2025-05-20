#pragma once


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

