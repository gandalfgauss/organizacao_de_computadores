
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <ctime>
#include "funcoes.hpp"

using namespace std;


void soma(long long int x, long long int y);
void initialize_rom();

void mem_read(int address, memdata_t* b, int* access_time);
void mem_write(int address, memdata_t* b, int* access_time);
int mem_size();
void disk_in(port_t port, memdata_t* value, int* access_time);
void disk_out(port_t port, memdata_t* value, int* access_time);


void cpu_interrupt(interruption_t type);
void cpu_save_state(int* access_time);
void cpu_restore_state(int* access_time);

memdata_t stringify(int number)
{
    memdata_t x;
    sprintf(x.data, "%d", number);
    return x;
}

typedef struct {

    int mar;
    int pc;
    int sp;

    memdata_t mbr;
    memdata_t ir;
    memdata_t a;

    interruption_t interruption;
} cpu_t;
cpu_t cpu;


#define RAM_SIZE 4096
#define ROM_SIZE 1024
#define USER_PROGRAM 1
#define BIOS_PROGRAM 512
#define INTERRUPTION 768
memdata_t rom[ROM_SIZE];

memdata_t  ram[RAM_SIZE];

typedef struct
{
  char data_register[MAXLEN+1];
  int port_0;
  int port_1;
  int port_2;
  int port_3;
  memdata_t disk_rig[RAM_SIZE*8];

}disk_control_t;

#define SEEK 1
#define READ 2
disk_control_t disk;

int main()
{
    initialize_rom();
    int access_time = 0;

    for(int i = 0; i < ROM_SIZE; i++)
        mem_write(i, &rom[i], &access_time);

    cpu.pc = 0;
    cpu.sp = mem_size();

    while(cpu.pc != -1 ) {

        cout << "\nProgran Counter " << cpu.pc << ": ";

        cpu.mar = cpu.pc;
        mem_read(cpu.mar, &cpu.mbr, &access_time);
        cpu.ir = cpu.mbr;
        cpu.pc += 1;

        if(cpu.pc >  INTERRUPTION + 1)
        {
          cout <<"\nDados CPU INTERRUPÇÃO:\nMAR = " << cpu.mar;
          cout <<"\nProgram Counter = " << cpu.pc;
          cout <<"\nStack Pointer = " <<  cpu.sp;
          cout <<"\nReg A = "<< cpu.a.data << endl;
        }

        if(cpu.pc == 526)
        {
            long long int x, y;

            srand(time(NULL));

            x = rand()%1000;
            y = rand()%1000;

            cout << x << " + " << y << " = ";

            soma(x,y);
        }

        switch(cpu.ir.code.opcode) {
            case OP_PRINT:
                cpu.mar = cpu.ir.code.operand;
                mem_read(cpu.mar, &cpu.mbr, &access_time);
                printf("%s\n", cpu.mbr.data);

                break;

            case OP_LOAD:
                cpu.mar = cpu.ir.code.operand;
                mem_read(cpu.mar, &cpu.mbr, &access_time);
                cpu.a = cpu.mbr;
                break;

            case OP_STORE:
                cpu.mar = cpu.ir.code.operand;
                cpu.mbr = cpu.a;
                mem_write(cpu.mar, &cpu.mbr, &access_time);
                break;

            case OP_JUMP:

                cpu.pc = cpu.ir.code.operand;
                break;

            case OP_MOV:
                cpu.a = stringify(cpu.ir.code.operand);
                break;

            case OP_IN: {
                port_t port = (port_t)cpu.ir.code.operand;
                disk_in(port, &cpu.a, &access_time);
                break;
            }

            case OP_OUT: {
                port_t port = (port_t)  cpu.ir.code.operand;
                disk_out(port, &cpu.a, &access_time);
                break;
            }

            case OP_IRET:
                cpu_restore_state(&access_time);
                break;

            case OP_CONCAT:
                cpu.mar = cpu.ir.code.operand;
                mem_read(cpu.mar, &cpu.mbr, &access_time);
                snprintf(cpu.a.data, sizeof(cpu.a.data), "%s%s", cpu.a.data, cpu.mbr.data);
                break;

            case OP_CLR:
                strcpy(cpu.a.data, "0");
                break;

            case OP_JZ:
                if(atoi(cpu.a.data) == 0)
                    cpu.pc = cpu.ir.code.operand;
                break;

            case OP_JL:
                if(atoi(cpu.a.data) < 0)
                    cpu.pc = cpu.ir.code.operand;
                break;

            case OP_JH:
                if(atoi(cpu.a.data) > 0)
                    cpu.pc = cpu.ir.code.operand;
                break;

            case OP_ADD:
                cpu.mar = cpu.ir.code.operand;
                mem_read(cpu.mar, &cpu.mbr, &access_time);
                cpu.a = stringify(atoi(cpu.a.data) + atoi(cpu.mbr.data));
                break;

            case OP_SUB:
                cpu.mar = cpu.ir.code.operand;
                mem_read(cpu.mar, &cpu.mbr, &access_time);
                cpu.a = stringify(atoi(cpu.a.data) - atoi(cpu.mbr.data));
                break;
        }

        if(cpu.interruption) {

           cout <<"\nINTERRUPÇÃO !" << endl;
           cout <<"\nSalvando dados " << endl;
           cout <<"\nDados CPU:\nMAR = " << cpu.mar;
           cout <<"\nProgram Counter = " << cpu.pc;
           cout <<"\nStack Pointer = " <<  cpu.sp;
           cout <<"\nReg A = "<< cpu.a.data;

           cpu_save_state(&access_time);
           cout <<"\nDados Salvos " << endl;

           cout <<"\nResolvendo..." << endl;

           cpu.pc = INTERRUPTION + (int)cpu.interruption;
           cpu.interruption = INT_ZERO;

        }
    }

    return 0;
}

void cpu_interrupt(interruption_t type)
{
    cpu.interruption = type;
}

void cpu_save_state(int* access_time)
{
    memdata_t mar = stringify(cpu.mar);
    memdata_t mbr = cpu.mbr;
    memdata_t pc = stringify(cpu.pc);
    memdata_t sp = stringify(cpu.sp);
    memdata_t ir = cpu.ir;
    memdata_t a = cpu.a;

    mem_write(--cpu.sp, &mar, access_time);
    mem_write(--cpu.sp, &mbr, access_time);
    mem_write(--cpu.sp, &pc, access_time);
    mem_write(--cpu.sp, &sp, access_time);
    mem_write(--cpu.sp, &ir, access_time);
    mem_write(--cpu.sp, &a, access_time);
}

void cpu_restore_state(int* access_time)
{
    memdata_t mar, mbr, pc, sp, ir, a;

    mem_read(cpu.sp++, &a, access_time);
    mem_read(cpu.sp++, &ir, access_time);
    mem_read(cpu.sp++, &sp, access_time);
    mem_read(cpu.sp++, &pc, access_time);
    mem_read(cpu.sp++, &mbr, access_time);
    mem_read(cpu.sp++, &mar, access_time);

    cpu.mar = atoi(mar.data);
    cpu.mbr = mbr;
    cpu.pc = atoi(pc.data);
    cpu.sp = atoi(sp.data);
    cpu.ir = ir;
    cpu.a = a;

    cout <<"\nDados CPU RESTAURADOS:\nMAR = " << cpu.mar;
    cout <<"\nProgram Counter = " << cpu.pc;
    cout <<"\nStack Pointer = " <<  cpu.sp;
    cout <<"\nReg A = "<< cpu.a.data << endl;
}

void initialize_rom()
{
  rom[ 0 ].code = { OP_JUMP, BIOS_PROGRAM };

  rom[ BIOS_PROGRAM + 0 ].code = { OP_CLR };

  rom[ BIOS_PROGRAM + 1 ].code = { OP_MOV, 0 };
  rom[ BIOS_PROGRAM + 2 ].code = { OP_OUT, 2 };

  rom[ BIOS_PROGRAM + 3 ].code = { OP_MOV, 1 };
  rom[ BIOS_PROGRAM + 4 ].code = { OP_OUT, PORT_DISK_CMD };

  rom[ BIOS_PROGRAM + 5 ].code = { OP_MOV, USER_PROGRAM };
  rom[ BIOS_PROGRAM + 6 ].code = { OP_OUT, PORT_DMA_ADDR };

  rom[ BIOS_PROGRAM + 7 ].code = { OP_MOV, 2 };
  rom[ BIOS_PROGRAM + 8 ].code = { OP_OUT, 0 };

  rom[ BIOS_PROGRAM + 9 ].code = { OP_IN, PORT_DISK_STATUS };
  rom[ BIOS_PROGRAM + 10 ].code = { OP_JH, BIOS_PROGRAM + 9 };

  rom[ BIOS_PROGRAM + 11 ].code = { OP_PRINT, BIOS_PROGRAM + 100 };
  rom[ BIOS_PROGRAM + 12 ].code = { OP_PRINT, BIOS_PROGRAM + 101 };
  rom[ BIOS_PROGRAM + 13 ].code = { OP_JUMP, USER_PROGRAM };

  strcpy(rom[ BIOS_PROGRAM + 100 ].data, "Booting...");
  strcpy(rom[ BIOS_PROGRAM + 101 ].data,"Programa carregado");


  rom[ INTERRUPTION + INT_ZERO ].code = { OP_IRET };
  rom[ INTERRUPTION + INT_DISK ].code = { OP_JUMP, INTERRUPTION + 100 };


  rom[ INTERRUPTION + 100 ].code = { OP_IN, 1 };
  rom[ INTERRUPTION + 101 ].code = { OP_JZ, INTERRUPTION + 105 };
  rom[ INTERRUPTION + 102 ].code = { OP_PRINT, INTERRUPTION + 109 };
  rom[ INTERRUPTION + 103 ].code = { OP_JUMP, -1 };
  rom[ INTERRUPTION + 105 ].code = { OP_IRET };
  strcpy(rom[ INTERRUPTION + 109 ].data,"falha de disco.");


}

void mem_read(int address, memdata_t* b, int* access_time)
{
  *b = ram[address];

}
void mem_write(int address, memdata_t* b, int* access_time)
{
  ram[address] = *b;
}

int mem_size()
{
  return RAM_SIZE;
}

void disk_in(port_t port, memdata_t* value, int* access_time)
{
  if((int)port == PORT_DISK_STATUS)
  {
    if(atoi(value->data) != 0)
    {   cout <<"\nDisco Ocupado";

    for(int dados = 1024; dados < 2048; dados++)
       mem_write(dados, &rom[dados], access_time);

      strcpy(value->data, "0");
    }

    if(atoi(value->data) == 0)
      cout <<"\nSucesso" << endl;
  }

}

void disk_out(port_t port, memdata_t* value, int* access_time)
{
  switch((int)port)
  {
    case PORT_DISK_CMD:

        switch(atoi(value->data))
        {
          case SEEK:
              disk.port_0 = atoi(value->data);
              cout <<"\nCabeçotes posicionados" << endl;

          break;

          case READ:

              cout <<"\nLeitura " << endl;
              disk_in(PORT_DISK_STATUS, value, access_time);

          break;
        }
    break;

    case PORT_DISK_DATA:


      disk.port_2 = atoi(value->data);
      strcpy(disk.data_register, value->data);

      cout <<"\nSolicitado: Ler bloco "<< atoi(value->data) << " do disco" << endl;

    break;

    case PORT_DMA_ADDR:

        disk.port_3 = atoi(value->data);

    break;
  }
}
void soma(long long int x,long long int y)
{

  char num1[64], num2[64];
  sprintf(num1, "%d", x);
  sprintf(num2, "%d", y);

  strcpy(ram[105].data,num1);
  strcpy(ram[106].data,num2);

  ram [1 ].code = { OP_LOAD, 105 };
  ram[ 2 ].code = { OP_ADD, 106 };
  ram[ 3 ].code = { OP_STORE, 107 };
  ram[ 4 ].code = { OP_PRINT, 107 };
  ram[ 5 ].code = { OP_JUMP, -1 };


  cpu_interrupt(INT_DISK);

}
