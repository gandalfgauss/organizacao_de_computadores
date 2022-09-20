#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>

using namespace std;

#define MAXIMO 63

#define TAMANHO_DA_MEMORIA        32768
#define TAMANHO_DO_BLOCO          64
#define NUMERO_DE_BLOCOS          (TAMANHO_DA_MEMORIA / TAMANHO_DO_BLOCO)

typedef struct Mem {
    int tempo_de_acesso;
    int numero_de_acessos;
    int byte[TAMANHO_DA_MEMORIA];
}Tmem;

#define TAMANHO_MAX_CACHE   TAMANHO_DA_MEMORIA
#define L1                  0
#define L2                  1
#define L3                  2
#define RAM                 3

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
    int dado;
} memdata_t;

typedef struct Tag
{
  int tag;
  int validade;
  int contador;
  int byte[TAMANHO_DO_BLOCO];
}Ttag_a;

typedef struct Cachee{
    int tamanho;
    int tempo_de_acesso_cache;
    int hits;
    int misses;
    Ttag_a blocos[10000];
    memdata_t cache_cpu[512];

}Tcache;

typedef struct {

    int mar;
    int pc;
    int dado;
    int mbr;
    memdata_t cache_mbr;
    int reg_a;
    int reg_b;

    memdata_t ir;

    memdata_t a;
} cpu_t;

cpu_t cpu = { cpu.pc = 0  };

void soma();
void inicializa(Tcache cache[3], Tmem& ram);
int selecionar_bloco_cache(int nivel,Tcache cache[3]);
void aumentar_contadores(int nivel, int numero_do_bloco, Tcache cache[3]);
int cache_transfere_bloco(int nivel, int para_bloco, int do_bloco,Tcache cache[3], Tmem& ram);
void mem_map(int nivel, int endereco, int* dado, int* num_bloco, int* tempo_ace, Tcache cache[3], Tmem& ram);
void poli_escrita(int nivel, int endereco, int dado, Tcache cache[3], Tmem& ram);

int main()
{
  srand(time(NULL));
  long long int x, y, decrescer, contador, adicionar;
  double acessos_l1, acessos_l2, acessos_l3, hits_l1, hits_l2, hits_l3, hitst, missest, misses_l1, misses_l2, misses_l3;
  double hitrate, missrate, missrate_l1, missrate_l2, missrate_l3;

  int endereco = 0, dado, num_bloco, tempo_ace, opcao;
  Tmem ram;
  Tcache cache[3];
  inicializa(cache,ram);

  for(int contador_cache = 0; contador_cache  < 280; contador_cache++)
  {
    if(contador_cache/20 == 0)
      endereco += TAMANHO_DO_BLOCO;

    else if((contador_cache/20) % 2 !=0)
      endereco -= TAMANHO_DO_BLOCO;

      else
        endereco += TAMANHO_DO_BLOCO;

    contador_cache++;

    cpu.mar = endereco;

    cout <<"\nEndereço "<< endereco << endl;

    mem_map(L1, cpu.mar, &dado, &num_bloco, &tempo_ace, cache, ram);

    cpu.mbr = dado;
    cpu.reg_a = cpu.mbr;

  }

  hits_l1 = cache[L1].hits;
  hits_l2 = cache[L2].hits;
  hits_l3 = cache[L3].hits;
  hitst  =   hits_l1 +  hits_l2 + hits_l3;

  misses_l1 = cache[L1].misses;
  misses_l2 = cache[L2].misses;
  misses_l3 = cache[L3].misses;
  missest  =   misses_l1 +  misses_l2 + misses_l3;


  acessos_l1 = hits_l1 + misses_l1;
  acessos_l2 = hits_l2 + misses_l2;
  acessos_l3 = hits_l3 + misses_l3;

  missrate_l1 = misses_l1/acessos_l1;
  missrate_l2 = misses_l2/acessos_l2;
  missrate_l3 = misses_l3/acessos_l3;

  missrate =   missrate_l1 * missrate_l2 * missrate_l3;
  hitrate = 1 - missrate;

  cout <<"\nNúmero total de cache hits: " << hitst  << endl;
  cout <<"\nNúmero total de cache misses: " << missest  << endl;
  cout <<"\nNúmero de acessos na RAM: " << ram.numero_de_acessos << endl;
  cout <<"\nHit rate " << hitrate << endl;
  cout <<"\nMiss rate " <<  missrate << endl << endl << endl;

  int endereco2;

  do
  {
    cout <<"\nDigite uma opcão: \n1-Soma\n2-Subtração\n3-Multiplicação\n4-Divisão"<< endl;
    cin >> opcao;

    switch(opcao)
    {
      case 1:

            endereco2 = -1;

            while(endereco2 < 0 || endereco2 < 650)
              endereco2 = rand()%1280;

            cpu.mar = endereco2;

            cout <<"\nDado x " << endl;
            mem_map(L1, cpu.mar, &dado, &num_bloco, &tempo_ace, cache, ram);
            x = dado;

            cout <<"\nDado y " << endl;
            mem_map(L1, cpu.mar+TAMANHO_DO_BLOCO, &dado, &num_bloco, &tempo_ace, cache, ram);
            y = dado;

            cout << x << " + " << y << " = ";

            cache[L1].cache_cpu[105].dado = x;
            cache[L1].cache_cpu[106].dado = y;

            cache[L1].cache_cpu[0].instrucao = { OP_LOAD, 105 };
            cache[L1].cache_cpu[1].instrucao = { OP_ADD, 106 };
            cache[L1].cache_cpu[2].instrucao = { OP_STORE, 107 };
            cache[L1].cache_cpu[3].instrucao = { OP_PRINT, 107 };
            cache[L1].cache_cpu[4].instrucao = { OP_JUMP, -1 };

            cpu.mbr = x+y;

            poli_escrita(L1, endereco2+2,cpu.mbr , cache, ram);


      break;

      case 2:

          endereco2 = -1;

          while(endereco2 < 0 || endereco2 < 650)
            endereco2 = rand()%1280;

          cpu.mar = endereco2;

          cout <<"\nDado x " << endl;
          mem_map(L1,cpu.mar, &dado, &num_bloco, &tempo_ace, cache, ram);
          x = dado;

          cpu.mar = endereco2;

          cout <<"\nDado y " << endl;
          mem_map(L1, cpu.mar+TAMANHO_DO_BLOCO, &dado, &num_bloco, &tempo_ace, cache, ram);
          y = dado;

              cout << x << " - " << y << " = ";

              cache[L1].cache_cpu[105].dado = x;
              cache[L1].cache_cpu[106].dado = y;

            cache[L1].cache_cpu[0].instrucao = { OP_LOAD, 105 };
            cache[L1].cache_cpu[1].instrucao = { OP_SUB, 106 };
            cache[L1].cache_cpu[2].instrucao = { OP_STORE, 107 };
            cache[L1].cache_cpu[3].instrucao = { OP_PRINT, 107 };
            cache[L1].cache_cpu[4].instrucao = { OP_JUMP, -1 };

            cpu.mbr = x-y;

          poli_escrita(L1, endereco2+2,cpu.mbr , cache, ram);

      break;

      case 3:

        x = rand()%1000;
        y = rand()%1000;

        cpu.mar = endereco2;

        decrescer = 1;

        cout << x << " * " << y << " = ";


        cache[L1].cache_cpu[100].dado = x;
        contador = y;

        cache[L1].cache_cpu[101].dado = contador;
        cache[L1].cache_cpu[102].dado = decrescer;
        cache[L1].cache_cpu[103].dado = x;

        cache[L1].cache_cpu[ 0 ].instrucao = { OP_LOAD, 101  };
        cache[L1].cache_cpu[ 1 ].instrucao = {OP_SUB, 102};
        cache[L1].cache_cpu[ 2 ].instrucao = { OP_JZ, 10  };
        cache[L1].cache_cpu[ 3 ].instrucao = {OP_LOAD, 100};
        cache[L1].cache_cpu[ 4 ].instrucao = {OP_ADD, 103};
        cache[L1].cache_cpu[ 5 ].instrucao = {OP_STORE, 103};
        cache[L1].cache_cpu[ 6 ].instrucao = {OP_LOAD, 101};
        cache[L1].cache_cpu[ 7 ].instrucao = {OP_SUB, 102};
        cache[L1].cache_cpu[ 8 ].instrucao = {OP_STORE, 101};
        cache[L1].cache_cpu[ 9 ].instrucao = { OP_JUMP, 1 };
        cache[L1].cache_cpu[ 10 ].instrucao = {OP_PRINT, 103};
        cache[L1].cache_cpu[ 11 ].instrucao = {OP_JUMP, -1};

        cpu.mbr = x*y;

        poli_escrita(L1, cpu.mar+TAMANHO_DO_BLOCO,cpu.mbr , cache, ram);

      break;

      case 4:

            contador = 0;
            adicionar = 1;

            cpu.mar = endereco2;

            x = rand()%1000;
            y = rand()%1000;



            cout << x << " / " << y << " = ";

          cache[L1].cache_cpu[100].dado = x;
          cache[L1].cache_cpu[101].dado = y;
          cache[L1].cache_cpu[102].dado = contador;
          cache[L1].cache_cpu[103].dado = adicionar;

            cache[L1].cache_cpu[ 0 ].instrucao = { OP_LOAD, 100  };
            cache[L1].cache_cpu[ 1 ].instrucao = {OP_SUB, 101};
            cache[L1].cache_cpu[ 2 ].instrucao = { OP_JL, 9  };
            cache[L1].cache_cpu[ 3 ].instrucao = {OP_STORE, 100};
            cache[L1].cache_cpu[ 4 ].instrucao = {OP_LOAD, 102};
            cache[L1].cache_cpu[ 5 ].instrucao = {OP_ADD, 103};
            cache[L1].cache_cpu[ 6 ].instrucao = {OP_STORE, 102};
            cache[L1].cache_cpu[ 7 ].instrucao = {OP_LOAD, 100};
            cache[L1].cache_cpu[ 8 ].instrucao = { OP_JUMP, 1 };
            cache[L1].cache_cpu[ 9 ].instrucao = {OP_PRINT, 102};
            cache[L1].cache_cpu[ 10 ].instrucao = {OP_JUMP, -1};

            cpu.mbr = x/y;

            poli_escrita(L1, cpu.mar+TAMANHO_DO_BLOCO*100,cpu.mbr , cache, ram);

      break;
    }

    cpu.pc = 0;

    while(cpu.pc != -1)
    {

      cpu.mar = cpu.pc;
      cpu.cache_mbr = cache[L1].cache_cpu[cpu.mar];
      cpu.ir = cpu.cache_mbr;
      cpu.pc += 1;

      switch(cpu.ir.instrucao.opcode) {
        case OP_PRINT:

              cpu.mar = cpu.ir.instrucao.operando;
              cpu.cache_mbr = cache[L1].cache_cpu[cpu.mar];
              cout << cpu.cache_mbr.dado << endl;

        break;

        case OP_LOAD:

              cpu.mar = cpu.ir.instrucao.operando;
              cpu.cache_mbr = cache[L1].cache_cpu[cpu.mar];
              cpu.a = cpu.cache_mbr;

        break;

        case OP_STORE:

              cpu.cache_mbr = cpu.a;
              cpu.mar = cpu.ir.instrucao.operando;
              cache[L1].cache_cpu[cpu.mar] = cpu.cache_mbr;

        break;

        case OP_CLR:

              cpu.a.dado = 0;

        break;

        case OP_JZ:
              if(cpu.a.dado == 0)
                cpu.pc = cpu.ir.instrucao.operando;
        break;

        case OP_JL:
              if((cpu.a.dado) < 0)
                cpu.pc = cpu.ir.instrucao.operando;
        break;

        case OP_JH:
              if((cpu.a.dado) > 0)
                    cpu.pc = cpu.ir.instrucao.operando;
        break;

        case OP_JUMP:
              cpu.pc = cpu.ir.instrucao.operando;
        break;

        case OP_ADD:
              cpu.mar = cpu.ir.instrucao.operando;
              cpu.cache_mbr = cache[L1].cache_cpu[cpu.mar];
              cpu.a.dado = cpu.a.dado + cpu.cache_mbr.dado;

        break;

        case OP_SUB:
              cpu.mar = cpu.ir.instrucao.operando;
              cpu.cache_mbr = cache[L1].cache_cpu[cpu.mar];
              cpu.a.dado = cpu.a.dado - cpu.cache_mbr.dado;
        break;

        case OP_MUL:
                cpu.mar = cpu.ir.instrucao.operando;
                cpu.cache_mbr = cache[L1].cache_cpu[cpu.mar];
                cpu.a.dado = cpu.a.dado * cpu.cache_mbr.dado;
        break;

        case OP_DIV:
            cpu.mar = cpu.ir.instrucao.operando;
            cpu.cache_mbr = cache[L1].cache_cpu[cpu.mar];
            cpu.a.dado = cpu.a.dado / cpu.cache_mbr.dado;
        break;
      }
    }

  }while(opcao != 0);


  return 0;
}

void inicializa(Tcache cache[3], Tmem& ram)
{
  cache[L1].tamanho =  1024 ;
  cache[L1].tempo_de_acesso_cache = 2;
  cache[L1].hits = 0;
  cache[L1].misses = 0;

  for(int block = 0; block < cache[L1].tamanho/TAMANHO_DO_BLOCO;block++)
  {
    cache[L1].blocos[block].validade = 0;
    cache[L1].blocos[block].contador = 0;
    cache[L1].blocos[block].tag = 0;

    for(int bytes = 0; bytes < TAMANHO_DO_BLOCO; bytes++)
    {
        cache[L1].blocos[block].byte[bytes] = 0;
    }
  }

  cache[L2].tamanho =  2048;
  cache[L2].tempo_de_acesso_cache = 16;
  cache[L2].hits = 0;
  cache[L2].misses = 0;

  for(int block = 0; block < cache[L2].tamanho/TAMANHO_DO_BLOCO;block++)
  {
    cache[L2].blocos[block].validade = 0;
    cache[L2].blocos[block].contador = 0;
    cache[L2].blocos[block].tag = 0;

    for(int bytes = 0; bytes < TAMANHO_DO_BLOCO; bytes++)
    {
        cache[L2].blocos[block].byte[bytes] = 0;
    }
  }

  cache[L3].tamanho =  4096;
  cache[L3].tempo_de_acesso_cache = 32;
  cache[L3].hits = 0;
  cache[L3].misses = 0;

  for(int block = 0; block < cache[L3].tamanho/TAMANHO_DO_BLOCO;block++)
  {
    cache[L3].blocos[block].validade = 0;
    cache[L3].blocos[block].contador = 0;
    cache[L3].blocos[block].tag = 0;

    for(int bytes = 0; bytes < TAMANHO_DO_BLOCO; bytes++)
    {
        cache[L3].blocos[block].byte[bytes] = 0;
    }
  }

  ram.tempo_de_acesso = 192;
  ram.numero_de_acessos = 0;

  for(int bytes = 0; bytes < TAMANHO_DA_MEMORIA; bytes++)
  {
      ram.byte[bytes] = bytes;
  }
}

int selecionar_bloco_cache(int nivel, Tcache cache[3])
{
  int num_blocos = cache[nivel].tamanho / TAMANHO_DO_BLOCO;

  int maior = 0;
  for(int block = 0; block < num_blocos; block++)
  {
    if(cache[nivel].blocos[block].validade == 0)
      return block;

    else if(cache[nivel].blocos[block].contador >= cache[nivel].blocos[maior].contador)
      maior = block;
  }
  return maior;
}

void aumentar_contadores(int nivel, int numero_do_bloco, Tcache cache[3])
{
    int num_blocos = cache[nivel].tamanho /TAMANHO_DO_BLOCO;

    for(int bloco = 0; bloco < num_blocos; bloco++)
        cache[nivel].blocos[bloco].contador++;

    cache[nivel].blocos[numero_do_bloco].contador = 0;
}

int cache_transfere_bloco(int nivel, int para_bloco, int do_bloco,Tcache cache[3], Tmem& ram)
{
    if(nivel + 1 >= RAM) {

        cout <<"\nAcesso a RAM"<< endl;
        ram.numero_de_acessos++;

        int ram_base_addr = do_bloco * TAMANHO_DO_BLOCO;
        int cache_base_addr = para_bloco * TAMANHO_DO_BLOCO;

        for(int offset = 0; offset < TAMANHO_DO_BLOCO; offset++) {
            int ram_byte_addr = ram_base_addr + offset;
            int cache_byte_addr = cache_base_addr + offset;
            cache[nivel].blocos[para_bloco].byte[cache_byte_addr] = ram.byte[ram_byte_addr];
        }
        cache[nivel].blocos[para_bloco].tag = do_bloco;
        cache[nivel].blocos[para_bloco].validade = 1;

        return para_bloco;
    }
    else {

        int my_base_addr = para_bloco * TAMANHO_DO_BLOCO;
        int other_base_addr = do_bloco * TAMANHO_DO_BLOCO;

        for(int offset = 0; offset < TAMANHO_DO_BLOCO; offset++) {
            int my_byte_addr = my_base_addr + offset;
            int other_byte_addr = other_base_addr + offset;

            cache[nivel].blocos[para_bloco].byte[my_byte_addr] = cache[nivel + 1].blocos[do_bloco].byte[other_byte_addr];
        }
        cache[nivel].blocos[para_bloco].tag = cache[nivel + 1].blocos[do_bloco].tag;
        cache[nivel].blocos[para_bloco].validade = 1;

        return para_bloco;
    }
}

void mem_map(int nivel, int endereco, int* dado, int* num_bloco, int* tempo_ace, Tcache cache[3], Tmem& ram)
{
     int tag_endereco = endereco/64;
     int offset_endereco = endereco%64;
     int encontrado = 0;
     int temp_aux;
     int block_aux = -1;
     int subs2, subs1, subs0, total_block;

    while(encontrado != 1)
    {
     total_block = cache[nivel].tamanho/TAMANHO_DO_BLOCO;

     for(int bloco = 0; bloco < total_block; bloco++)
     {
        temp_aux++;

         if(cache[nivel].blocos[bloco].tag == tag_endereco && cache[nivel].blocos[bloco].validade != 0)
         {

           cout <<"\nCache hit ! Na cache " << nivel+1 << endl;


            *dado = cache[nivel].blocos[bloco].byte[offset_endereco];

            block_aux = bloco;
            total_block = 0;
            tempo_ace = &temp_aux;
            encontrado = 1;

            aumentar_contadores(nivel, bloco, cache);

            cache[nivel].hits++;

            if(nivel == L3)
            {

              subs1 =  selecionar_bloco_cache(L2,cache);
              cache_transfere_bloco(L2, subs1,bloco,cache, ram);
              aumentar_contadores(L2, subs1, cache);

              subs0 =  selecionar_bloco_cache(L1,cache);
              cache_transfere_bloco(L1, subs0,subs1,cache, ram);
              aumentar_contadores(L1, subs0, cache);

            }

            if(nivel == L2)
            {

              subs0 =  selecionar_bloco_cache(L1,cache);
              cache_transfere_bloco(L1, subs0,bloco,cache, ram);
              aumentar_contadores(L1, subs0, cache);

            }
         }
     }

      num_bloco = &block_aux;

     if(encontrado == 0)
     {
        cache[nivel].misses++;

        cout << "\nCache miss ! Na cache " << nivel+1 << endl;

        if(nivel == L3)
        {

          subs2 = selecionar_bloco_cache(nivel,cache);
          cache_transfere_bloco(nivel, subs2,tag_endereco,cache, ram);
          aumentar_contadores(nivel, subs2, cache);

          subs1 =  selecionar_bloco_cache(L2,cache);
          cache_transfere_bloco(L2, subs1,subs2,cache, ram);
          aumentar_contadores(L2, subs1, cache);


          subs0 =  selecionar_bloco_cache(L1,cache);
          cache_transfere_bloco(L1, subs0,subs1,cache, ram);
          aumentar_contadores(L3, subs0, cache);

          *dado = ram.byte[endereco];

            encontrado = 1;
        }
        nivel++;

     }
   }
}

  void poli_escrita(int nivel, int endereco, int dado, Tcache cache[3], Tmem& ram)
  {

         int tag_endereco = endereco/64;
         int offset_endereco = endereco%64;
         int encontrado = 0;

         int subs1, subs2, subs3;

         int total_block = cache[nivel].tamanho/TAMANHO_DO_BLOCO;

         for(int block = 0; block < total_block; block++)
         {
             if(cache[nivel].blocos[block].tag == tag_endereco)
             {

               cout <<"\nWrite hit !" << endl;//write through

                total_block = 0;

                encontrado = 1;

                aumentar_contadores(nivel, block, cache);

                cache[L1].blocos[block].byte[offset_endereco] = dado;
                cache[L2].blocos[block].byte[offset_endereco] = dado;
                cache[L3].blocos[block].byte[offset_endereco] = dado;

                ram.byte[endereco] = dado;
              }
            }

         if(encontrado == 0)//write around
         {
            cout << "\nWrite miss !" << endl;

            subs1 = selecionar_bloco_cache(L1, cache);
            cache[L1].blocos[subs1].byte[offset_endereco] = dado;

            subs2 = selecionar_bloco_cache(L2, cache);
            cache[L2].blocos[subs2].byte[offset_endereco] = dado;

            subs3 = selecionar_bloco_cache(L3, cache);
            cache[L3].blocos[subs3].byte[offset_endereco] = dado;

            ram.byte[endereco] = dado;
          }
  }
