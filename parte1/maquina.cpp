#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>


using namespace std;

#define MAXIMO 63
void string_concatena(char* dest, const char* str1, const char* str2)
{
    int i = 0, j = 0, k = 0;

    while(i < MAXIMO && str1[j] != '\0') {
        dest[i] = str1[j];
        i++; j++;
    }
    while(i < MAXIMO && str2[k] != '\0') {
        dest[i] = str2[k];
        i++; k++;
    }
    dest[i] = '\0';
}

typedef enum {

    OP_PRINT = 0,
    OP_LOAD,
    OP_CONCAT,
    OP_STORE,

    OP_CLR,
    OP_JZ,
    OP_JL,
    OP_JUMP,
    OP_JH,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

} opcode_t;

typedef struct {
    opcode_t opcode;
    int operando;
} instruction_t;

typedef union {
    instruction_t instrucao;
    char dado[MAXIMO + 1];
} memdata_t;

typedef struct {

    int mar;
    int pc;

    memdata_t mbr;
    memdata_t ir;

    memdata_t a, b;
} cpu_t;

memdata_t mem[512];

cpu_t cpu = { cpu.pc = 0  };

void maquina();
void prog(int programa);

void concatena();
void soma();
void subtrai();
void multiplica();
void divisao();
void modulo();
void potencia();
void resto_div();
void tabuada();
void fatorial();
void pa();
void pg();
void loga();
void fibonacci();
void delta();

int main()
{
    int programa;

      srand(time(NULL));

      cout <<"Programas: " << endl;
      cout <<"1 - Concatena 2 números"<< endl;
      cout <<"2 - Soma 2 números" << endl;
      cout <<"3 - Subtrai 2 números" << endl;
      cout <<"4 - Multiplica 2 números" << endl;
      cout <<"5 - Divide 2 números" << endl;
      cout <<"6 - Módulo de um número" << endl;
      cout <<"7 - Potênciação" << endl;
      cout <<"8 - Resto da divisão" << endl;
      cout <<"9 - Tabuada" << endl;
      cout <<"10- Fatorial " << endl;
      cout <<"11- PA " << endl;
      cout <<"12- PG " << endl;
      cout <<"13- Logaritmo" << endl;
      cout <<"14- Fibonacci" << endl;
      cout <<"15- Calcular Delta" << endl;
      cout << "\nDigite o programa que deseja utilizar : " << endl;

      cin >> programa;

      prog(programa);

      maquina();

  return 0;
}

void maquina()
{
  while(cpu.pc != -1)
  {

    cpu.mar = cpu.pc;
    cpu.mbr = mem[cpu.mar];
    cpu.ir = cpu.mbr;
    cpu.pc += 1;

    switch(cpu.ir.instrucao.opcode) {
      case OP_PRINT:
            cpu.mar = cpu.ir.instrucao.operando;
            cpu.mbr = mem[cpu.mar];
            puts(cpu.mbr.dado);
      break;

      case OP_LOAD:
            cpu.mar = cpu.ir.instrucao.operando;
            cpu.mbr = mem[cpu.mar];
            cpu.a = cpu.mbr;
      break;

      case OP_STORE:
            cpu.mbr = cpu.a;
            cpu.mar = cpu.ir.instrucao.operando;
            mem[cpu.mar] = cpu.mbr;
      break;

      case OP_CONCAT:
            cpu.mar = cpu.ir.instrucao.operando;
            cpu.mbr = mem[cpu.mar];
            string_concatena(cpu.a.dado, cpu.a.dado, cpu.mbr.dado);
      break;

      case OP_CLR:
            cpu.a.dado[0] = '0';
            cpu.a.dado[1] = '\0';
      break;

      case OP_JZ:
            if(atoi(cpu.a.dado) == 0)
              cpu.pc = cpu.ir.instrucao.operando;
      break;

      case OP_JL:
            if(atoi(cpu.a.dado) < 0)
              cpu.pc = cpu.ir.instrucao.operando;
      break;

      case OP_JH:
            if(atoi(cpu.a.dado) > 0)
                  cpu.pc = cpu.ir.instrucao.operando;
      break;

      case OP_JUMP:
            cpu.pc = cpu.ir.instrucao.operando;
      break;

      case OP_ADD:
            cpu.mar = cpu.ir.instrucao.operando;
            cpu.mbr = mem[cpu.mar];
            sprintf(cpu.a.dado, "%d", atoi(cpu.a.dado) + atoi(cpu.mbr.dado));
      break;

      case OP_SUB:
            cpu.mar = cpu.ir.instrucao.operando;
            cpu.mbr = mem[cpu.mar];
            sprintf(cpu.a.dado, "%d", atoi(cpu.a.dado) - atoi(cpu.mbr.dado));
      break;

      case OP_MUL:
              cpu.mar = cpu.ir.instrucao.operando;
              cpu.mbr = mem[cpu.mar];
              sprintf(cpu.a.dado, "%d", atoi(cpu.a.dado) * atoi(cpu.mbr.dado));
      break;

      case OP_DIV:
            cpu.mar = cpu.ir.instrucao.operando;
            cpu.mbr = mem[cpu.mar];
            sprintf(cpu.a.dado, "%d", atoi(cpu.a.dado) / atoi(cpu.mbr.dado));
      break;
    }
  }
}

void prog(int programa)
{
  switch(programa)
  {
    case 1:
      concatena();
    break;

    case 2:
      soma();
    break;

    case 3:
      subtrai();
    break;

    case 4:
      multiplica();
    break;

    case 5:
      divisao();
    break;

    case 6:
      modulo();
    break;

    case 7:
      potencia();
    break;

    case 8:
      resto_div();
    break;

    case 9:
        tabuada();
    break;

    case 10:
        fatorial();
    break;

    case 11:
      pa();
    break;

    case 12:
      pg();
    break;

    case 13:
      loga();
    break;

    case 14:
      fibonacci();
    break;

    case 15:
      delta();
    break;
  }
}

void concatena()
{
  long long int x, y;

  x = rand()%1000;
  y = rand()%1000;

  cout << x << " concatena " << y << " = ";

  char string1[MAXIMO+1], string2[MAXIMO+1];
  sprintf(string1, "%d", x);
  sprintf(string2, "%d", y);

  strcpy(mem[100].dado,string1);
  strcpy(mem[101].dado,string2);

 mem[ 0 ].instrucao = { OP_LOAD, 100 };
 mem[ 1 ].instrucao = { OP_CONCAT, 101 };
 mem[ 2 ].instrucao = { OP_STORE, 100 };
 mem[ 3 ].instrucao = { OP_PRINT, 100 };
 mem[ 4 ].instrucao = { OP_JUMP, -1 };

}

void soma()
{
  long long int x, y;

  x = rand()%1000;
  y = rand()%1000;

  cout << x << " + " << y << " = ";

  char num1[MAXIMO+1], num2[MAXIMO+1];
  sprintf(num1, "%d", x);
  sprintf(num2, "%d", y);

  strcpy(mem[105].dado,num1);
  strcpy(mem[106].dado,num2);

  mem[ 0 ].instrucao = { OP_LOAD, 105 };
  mem[ 1 ].instrucao = { OP_ADD, 106 };
  mem[ 2 ].instrucao = { OP_STORE, 107 };
  mem[ 3 ].instrucao = { OP_PRINT, 107 };
  mem[ 4 ].instrucao = { OP_JUMP, -1 };
}

void subtrai()
{
  long long int x, y;

  x = rand()%1000;
  y = rand()%1000;

  cout << x << " - " << y << " = ";

  char num1[MAXIMO+1], num2[MAXIMO+1];
  sprintf(num1, "%d", x);
  sprintf(num2, "%d", y);

  strcpy(mem[105].dado,num1);
  strcpy(mem[106].dado,num2);

  mem[ 0 ].instrucao = { OP_LOAD, 105 };
  mem[ 1 ].instrucao = { OP_SUB, 106 };
  mem[ 2 ].instrucao = { OP_STORE, 107 };
  mem[ 3 ].instrucao = { OP_PRINT, 107 };
  mem[ 4 ].instrucao = { OP_JUMP, -1 };
}

void multiplica()
{
  char contador[MAXIMO], decrescer[MAXIMO] = "1";

  long long int x, y;

  x = rand()%100;
  y = rand()%100;

  cout << x << " * " << y << " = ";

  char num1[MAXIMO+1], num2[MAXIMO+1];
  sprintf(num1, "%d", x);
  sprintf(num2, "%d", y);

  strcpy(mem[100].dado,num1);
  strcpy(contador,num2);
  strcpy(mem[101].dado,contador);
  strcpy(mem[102].dado,decrescer);
  strcpy(mem[103].dado,num1);

  mem[ 0 ].instrucao = { OP_LOAD, 101  };
  mem[ 1 ].instrucao = {OP_SUB, 102};
  mem[ 2 ].instrucao = { OP_JZ, 10  };
  mem[ 3 ].instrucao = {OP_LOAD, 100};
  mem[ 4 ].instrucao = {OP_ADD, 103};
  mem[ 5 ].instrucao = {OP_STORE, 103};
  mem[ 6 ].instrucao = {OP_LOAD, 101};
  mem[ 7 ].instrucao = {OP_SUB, 102};
  mem[ 8 ].instrucao = {OP_STORE, 101};
  mem[ 9 ].instrucao = { OP_JUMP, 1 };
  mem[ 10 ].instrucao = {OP_PRINT, 103};
  mem[ 11 ].instrucao = {OP_JUMP, -1};

}

void divisao()
{
  char contador[MAXIMO] = "0", adicionar[MAXIMO] = "1";

  long long int x, y;

  x = rand()%1000;
  y = rand()%1000+1;

  cout << x << " / " << y << " = ";

  char num1[MAXIMO+1], num2[MAXIMO+1];
  sprintf(num1, "%d", x);
  sprintf(num2, "%d", y);

  strcpy(mem[100].dado,num1);
  strcpy(mem[101].dado,num2);
  strcpy(mem[102].dado,contador);
  strcpy(mem[103].dado,adicionar);

  mem[ 0 ].instrucao = { OP_LOAD, 100  };
  mem[ 1 ].instrucao = {OP_SUB, 101};
  mem[ 2 ].instrucao = { OP_JL, 9  };
  mem[ 3 ].instrucao = {OP_STORE, 100};
  mem[ 4 ].instrucao = {OP_LOAD, 102};
  mem[ 5 ].instrucao = {OP_ADD, 103};
  mem[ 6 ].instrucao = {OP_STORE, 102};
  mem[ 7 ].instrucao = {OP_LOAD, 100};
  mem[ 8 ].instrucao = { OP_JUMP, 1 };
  mem[ 9 ].instrucao = {OP_PRINT, 102};
  mem[ 10 ].instrucao = {OP_JUMP, -1};

}

void modulo()
{
  long long int x = (rand()/-1)%1000;
  char num1[MAXIMO];
  char y[MAXIMO] = "-1";

  cout << "| "<< x <<" | = ";
  sprintf(num1, "%d", x);

  strcpy(mem[110].dado,num1 );
  strcpy(mem[111].dado,y);


  mem[ 0 ].instrucao = { OP_LOAD, 110 };
  mem[ 1 ].instrucao = { OP_JH, 3 };
  mem[ 2 ].instrucao = {OP_MUL, 111};
  mem[ 3 ].instrucao = { OP_STORE, 112 };
  mem[ 4 ].instrucao = { OP_PRINT, 112 };
  mem[ 5 ].instrucao = { OP_JUMP, -1 };
}

void potencia()
{
  char contador[MAXIMO], decrescer[MAXIMO] = "1";

  long long int x, y;

  x = rand()%10+1;
  y = rand()%10+1;

  cout << x << " ^ " << y << " = ";

  char base[MAXIMO+1], expoente[MAXIMO+1];
  sprintf(base, "%d", x);
  sprintf(expoente, "%d", y);

  strcpy(mem[200].dado,base);
  strcpy(contador,expoente);
  strcpy(mem[202].dado,contador);
  strcpy(mem[203].dado,decrescer);
  strcpy(mem[204].dado,base);

  mem[ 0 ].instrucao = { OP_LOAD, 202  };
  mem[ 1 ].instrucao = {OP_SUB, 203};
  mem[ 2 ].instrucao = { OP_JZ, 10  };
  mem[ 3 ].instrucao = {OP_LOAD, 200};
  mem[ 4 ].instrucao = {OP_MUL, 204};
  mem[ 5 ].instrucao = {OP_STORE, 204};
  mem[ 6 ].instrucao = {OP_LOAD, 202};
  mem[ 7 ].instrucao = {OP_SUB, 203};
  mem[ 8 ].instrucao = {OP_STORE, 202};
  mem[ 9 ].instrucao = { OP_JUMP, 1 };
  mem[ 10 ].instrucao = {OP_PRINT, 204};
  mem[ 11 ].instrucao = {OP_JUMP, -1};

}

void resto_div()
{
  long long int x, y;

  x = rand()%100+1;
  y = rand()%100+1;

  cout <<"\nO resto da divisão de "<< x << " por " << y << " = ";

  char divisor[MAXIMO+1], dividendo[MAXIMO+1];
  sprintf(divisor, "%d", x);
  sprintf(dividendo, "%d", y);


  strcpy(mem[100].dado,divisor);
  strcpy(mem[101].dado,dividendo);

  mem[ 0 ].instrucao = { OP_LOAD, 100  };
  mem[ 1 ].instrucao = {OP_SUB, 101};
  mem[ 2 ].instrucao = { OP_JL, 8  };
  mem[ 3 ].instrucao = { OP_JZ, 7  };
  mem[ 4 ].instrucao = { OP_STORE, 100  };
  mem[ 5 ].instrucao = {OP_SUB, 101};
  mem[ 6].instrucao = { OP_JUMP, 2 };
  mem[ 7 ].instrucao = { OP_STORE, 100  };
  mem[ 8].instrucao = {OP_PRINT, 100};
  mem[ 9 ].instrucao = {OP_JUMP, -1};
}
void tabuada()
{
  char contador[MAXIMO] = "9", decrescer[MAXIMO] = "1", resultado[MAXIMO];

  long long int x;

  x = rand()%11;

  cout <<"Tabuada de " << x << endl;

  char num[MAXIMO+1];
  sprintf(num, "%d", x);

  strcpy(mem[100].dado,num);
  strcpy(mem[101].dado,contador);
  strcpy(mem[102].dado,decrescer);
  strcpy(resultado, num );
  strcpy(mem[103].dado,resultado);

  mem[ 0 ].instrucao = { OP_PRINT, 100  };
  mem[ 1 ].instrucao = { OP_LOAD, 101  };
  mem[ 2 ].instrucao = { OP_JZ, 10  };
  mem[ 3 ].instrucao = { OP_SUB,  102 };
  mem[ 4 ].instrucao = { OP_STORE,101 };
  mem[ 5 ].instrucao = { OP_LOAD, 103 };
  mem[ 6 ].instrucao = { OP_ADD, 100 };
  mem[ 7 ].instrucao = { OP_STORE, 103 };
  mem[ 8 ].instrucao = { OP_PRINT, 103 };
  mem[ 9 ].instrucao = { OP_JUMP, 1 };
  mem[ 10 ].instrucao = { OP_JUMP, -1 };
}

void fatorial()
{
  char contador[MAXIMO], decrescer[MAXIMO] = "1", resultado[MAXIMO] = "0", aux[MAXIMO] = "0";

  long long int x;

  x = rand()%11+1;

  cout << x << "! " << " = ";

  char fat[MAXIMO+1];

  sprintf(fat, "%d", x);

  strcpy(mem[100].dado,fat);
  strcpy(mem[101].dado,decrescer);
  strcpy(contador, fat);
  strcpy(mem[102].dado,contador);
  strcpy(mem[103].dado,aux);
  strcpy(mem[104].dado,resultado);

  mem[ 0 ].instrucao = { OP_LOAD, 102  };
  mem[ 1 ].instrucao = { OP_JL, 12 };
  mem[ 2 ].instrucao = { OP_JZ, 12  };
  mem[ 3 ].instrucao = { OP_STORE,  104 };
  mem[ 4 ].instrucao = { OP_SUB,101 };
  mem[ 5 ].instrucao = { OP_STORE,  102 };
  mem[ 6 ].instrucao = { OP_JL, 12 };
  mem[ 7 ].instrucao = { OP_JZ, 12  };
  mem[ 8 ].instrucao = { OP_MUL, 104 };
  mem[ 9 ].instrucao = { OP_STORE,  104 };
  mem[ 10 ].instrucao = { OP_LOAD, 102  };
  mem[ 11].instrucao = { OP_JUMP, 4 };
  mem[ 12 ].instrucao = { OP_PRINT, 104 };
  mem[ 13 ].instrucao = { OP_JUMP, -1 };
}

void pa()
{
  int razao, a1, numero_termos;
  char a1_2[MAXIMO+1];

  razao = rand()%11;
  a1 = rand()%100;
  numero_termos = rand()%20+1;

  cout <<"\nPA de razão: "<< razao << endl;
  cout <<"a1: " << a1 << endl;
  cout <<"Número de termos: " << numero_termos << endl;

  for(int i = 0; i < numero_termos; i++)
  {
      sprintf(a1_2, "%d", a1);
      strcpy(mem[i+100].dado,a1_2);
      mem[ i ].instrucao = { OP_PRINT, i+100 };

      a1 += razao;
  }

    mem[ numero_termos ].instrucao = { OP_JUMP, -1 };
}

void pg()
{
  long long int razao, a1, numero_termos;
  char a1_2[MAXIMO+1];

  razao = rand() % 12;
  a1 = rand() % 13;
  numero_termos = rand()% 9 +1;

  cout <<"\nPG de razão: "<< razao << endl;
  cout <<"a1: " << a1 << endl;
  cout <<"Número de termos: " << numero_termos << endl;

  for(int i = 0; i < numero_termos; i++)
  {
      sprintf(a1_2, "%d", a1);
      strcpy(mem[i+100].dado,a1_2);
      mem[ i ].instrucao = { OP_PRINT, i+100 };

      a1 *= razao;
  }

    mem[ numero_termos ].instrucao = { OP_JUMP, -1 };
}

void loga()
{
  long long int logarit, base, logaritmo;
  char logaritmo2[MAXIMO+1];

  logarit = rand()%20 + 1;
  base = rand()%10 + 1;

  while(base == 1)
  {
      base = rand()%10 + 1;
  }

  logaritmo = log10(logarit)/log10(base);

  cout <<"\nO logaritmo de "<< logarit <<" na base " << base <<" é : ";

  sprintf(logaritmo2, "%d", logaritmo);
  strcpy(mem[100].dado,logaritmo2);

  mem[ 0 ].instrucao = { OP_PRINT, 100 };
  mem[ 1 ].instrucao = { OP_JUMP, -1 };
}

void fibonacci()
{
  long long int numero_termos;
  int var1 = 0, var2 = 1, var3 = 1;

  char var3_2[MAXIMO+1];

  numero_termos = rand()% 50;

  cout <<"\nFibonacci: "<< endl;
  cout <<"Número de termos: " << numero_termos << endl;

  for(int i = 0; i < numero_termos; i++)
  {
      if(i == 0)
      {
          sprintf(var3_2, "%d", var3 );
          strcpy(mem[i+100].dado,var3_2);
          mem[ i ].instrucao = { OP_PRINT, i+100 };
      }

      else
      {
        sprintf(var3_2, "%d", var3 );
        strcpy(mem[i+100].dado,var3_2);
        mem[ i ].instrucao = { OP_PRINT, i+100 };

        var1 = var2;
        var2 = var3;
        var3 = var1+var2;
      }
   }
    mem[ numero_termos ].instrucao = { OP_JUMP, -1 };
}

void delta()
{
  long long int delta, a, b, c;
  char delta_2[MAXIMO+1];

  a = rand() % 20 +1;
  b = rand() % 20;
  c = rand()% 20;

  cout <<"\nTermo a: "<< a << endl;
  cout <<"Termo b: "<< b << endl;
  cout <<"Termo c: "<< c << endl;
  cout <<"Delta: ";
  delta = b*b -4*a*c;

      sprintf(delta_2, "%d", delta);
      strcpy(mem[100].dado,delta_2);
      mem[ 0 ].instrucao = { OP_PRINT, 100 };
      mem[ 1 ].instrucao = { OP_JUMP, -1 };
}
