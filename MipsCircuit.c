/* 
Class Project: The logical conclusion (v1.1)
CSCI-2500 Spring 2021
Prof. Slota 
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void convert_to_binary(int a, BIT* A, int length);
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* JumpReg);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void updateState();
  
/******************************************************************************/
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A)
{
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
  if (A || B)
    return TRUE;
  else
    return FALSE;
}

BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
}

BIT and_gate(BIT A, BIT B)
{
  if (A && B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}

BIT xor_gate(BIT A, BIT B)
{
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  // Note: The input -> output mapping is slightly modified from before
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);
  
  return;
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);  
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  for (int i = 0; i < 32; ++i) {
    BIT x0 = and_gate(not_gate(S), I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);
  
  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);
  
  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);
  
  return or_gate(z0, z1);  
}


/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i]; 
}

void print_binary(BIT* A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]); 
}

void convert_to_binary(int a, BIT* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? TRUE : FALSE);
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? TRUE : FALSE);
      a /= 2;
    }
  }
}

void convert_to_binary_char(int a, char* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? '1' : '0');
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? '1' : '0');
      a /= 2;
    }
  }
}
  
int binary_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;
  
  for (int i = 0; i < 32; ++i) {
    a += A[i]*mult;
    mult *= 2;
  }
  
  return (int)a;
}


/******************************************************************************/
/* Parsing functions */
/******************************************************************************/

// TODO: Implement any helper functions to assist with parsing
void set_register(char* input, char* output)
{
  // zero = 0
  if (strcmp(input, "zero") == 0)
    strncpy(output, "00000", 5);
  // v0 = 2
  if (strcmp(input, "v0") == 0)
    strncpy(output, "01000", 5);
  // a0 = 4
    if (strcmp(input, "a0") == 0)
    strncpy(output, "00100", 5);
  // t0 = 8
  if (strcmp(input, "t0") == 0)
    strncpy(output, "00010", 5);
  // t1 = 9
  else if (strcmp(input, "t1") == 0)
    strncpy(output, "10010", 5);
  // s0 = 16
  else if (strcmp(input, "s0") == 0)
    strncpy(output, "00001", 5);
  // s1 = 17
  else if (strcmp(input, "s1") == 0)
    strncpy(output, "10001", 5);
  // sp = 29
  if (strcmp(input, "sp") == 0)
    strncpy(output, "10111", 5);
  // ra = 31
  if (strcmp(input, "v0") == 0)
    strncpy(output, "11111", 5);
}

int get_instructions(BIT Instructions[][32])
{
  char line[256] = {0};
  int instruction_count = 0;
  while (fgets(line, 256, stdin) != NULL) {        
    // TODO: perform conversion of instructions to bianry
    // Input: coming from stdin via: ./a.out < input.txt
    // Output: Convert instructions to binary in the instruction memory
    // Return: Total number of instructions
    // Note: you are free to use if-else and external libraries here
    // Note: you don't need to implement circuits for saving to inst. mem.
    // My approach:
    // - Use sscanf on line to get strings for instruction and registers
    // - Use instructions to determine op code, funct, and shamt fields
    // - Convert immediate field and jump address field to binary
    // - Use registers to get rt, rd, rs fields
    // Note: I parse everything as strings, then convert to BIT array at end
    BIT output[32] = {FALSE};
    char inst[256] = {0};
    char op1[256] = {0};
    char op2[256] = {0};
    char op3[256] = {0};
    sscanf(line, "%s %s %s %s", inst, op1, op2, op3);
    
    char temp_output[33] = {0};
    char rs[6] = {0};
    char rt[6] = {0};
    char rd[6] = {0};
    char imm[17] = {0};
    char address[27] = {0};
    
    if (strcmp(inst, "lw") == 0) {
      // reg1 = M[reg2 + offset]
      convert_to_binary_char(atoi(op3), imm, 16);
      set_register(op1, rt);
      set_register(op2, rs);
      strncpy(&temp_output[0], imm, 16);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      // lw opcode = 35
      strncpy(&temp_output[26], "110001", 6);      
    }
    else if (strcmp(inst, "sw") == 0) {
      // M[reg2 + offset] = reg1
      convert_to_binary_char(atoi(op3), imm, 16);
      set_register(op1, rt);
      set_register(op2, rs);
      strncpy(&temp_output[0], imm, 16);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      // Is this the only part I change? Yes
      // sw opcode = 43
      strncpy(&temp_output[26], "110101", 6);
    }
    else if (strcmp(inst, "beq") == 0) {
      // if(reg1 == reg2) PC += constant
      // Do I need to do more to get the program counter, or can I still just convert the constant? Just need the constant according to TA
      convert_to_binary_char(atoi(op3), imm, 16);
      set_register(op1, rt);
      set_register(op2, rs);
      strncpy(&temp_output[0], imm, 16);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      // Is this it?
      // beq opcode = 4
      strncpy(&temp_output[26], "001000", 6);
    }
    else if (strcmp(inst, "addi") == 0) {
      // reg1 = reg2 + constant(op3)
      convert_to_binary_char(atoi(op3), imm, 16);
      // Do I make op1 an rd since it takes stuff in, but then where does the constant go? Don't need to worry. It's an I-type, so it just follows
      // the same conventions as the other I-types according to a TA
      // Is the op2 an rs or rt then? rs
      set_register(op1, rt);
      set_register(op2, rs);
      strncpy(&temp_output[0], imm, 16);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      //  opcode = 8
      strncpy(&temp_output[26], "000100", 6);
    }
    // Ignore the shamt code parts
    else if (strcmp(inst, "and") == 0) {
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      // funct = 36
      strncpy(&temp_output[0], "001001", 6);
      // shamt = ignore
      strncpy(&temp_output[6], "00000", 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      // opcode = 0
      strncpy(&temp_output[26], "000000", 6); 
    }
    else if (strcmp(inst, "or") == 0) {
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      // funct = 37
      strncpy(&temp_output[0], "101001", 6);
      // shamt = ignore
      strncpy(&temp_output[6], "00000", 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      // opcode = 0
      strncpy(&temp_output[26], "000000", 6);
    }
    else if (strcmp(inst, "add") == 0) {
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      // funct = 32
      strncpy(&temp_output[0], "000001", 6);
      // shamt = ignore
      strncpy(&temp_output[6], "00000", 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      // opcode = 0
      strncpy(&temp_output[26], "000000", 6);      
    }
    else if (strcmp(inst, "sub") == 0) {
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      // funct = 34
      strncpy(&temp_output[0], "010001", 6);
      // shamt = ignore
      strncpy(&temp_output[6], "00000", 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      // opcode = 0
      strncpy(&temp_output[26], "000000", 6);
    } 
    else if (strcmp(inst, "slt") == 0) {
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);
      // funct = 42
      strncpy(&temp_output[0], "010101", 6);
      // shamt = ignore?
      strncpy(&temp_output[6], "00000", 5);
      strncpy(&temp_output[11], rd, 5);
      strncpy(&temp_output[16], rt, 5);
      strncpy(&temp_output[21], rs, 5);
      // opcode = 0
      strncpy(&temp_output[26], "000000", 6);
    }
    else if (strcmp(inst, "j") == 0) {
      convert_to_binary_char(atoi(op1), address, 26);
      strncpy(&temp_output[0], address, 26);
      // opcode = 2
      strncpy(&temp_output[26], "010000", 6);      
    }
    else if (strcmp(inst, "jal") == 0) {
      convert_to_binary_char(atoi(op1), address, 26);
      strncpy(&temp_output[0], address, 26);
      // opcode = 3
      strncpy(&temp_output[26], "110000", 6);
    }
    else if (strcmp(inst, "jr") == 0) {
      // https://personal.ntu.edu.sg/smitha/FYP_Gerald/jrInstruction.html
      set_register(op1, rs);
      // Why is there another register?
      strncpy(&temp_output[11], rs, 5);
      strncpy(&temp_output[21], rs, 5);
      // funct code = 8
      strncpy(&temp_output[0], "000100", 6);
    }
    for (int i = 0; i < 32; ++i)
      output[i] = (temp_output[i] == '1' ? TRUE : FALSE); 
    // Storing the result
    copy_bits(output, Instructions[instruction_count]);
    /*
    printf("Instruction: ");
    for (int i = 31; i >= 0; --i) {
      printf("%d", output[i]);
    }
    printf("\n");
    */
    ++instruction_count;
  }
  
  return instruction_count;
}


/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32]                  = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE};
BIT MEM_Data[32][32]        = {FALSE};
BIT MEM_Register[32][32]    = {FALSE};

BIT RegDst    = FALSE;
BIT Jump      = FALSE;
BIT Branch    = FALSE;
BIT MemRead   = FALSE;
BIT MemToReg  = FALSE;
BIT ALUOp[2]  = {FALSE};
BIT MemWrite  = FALSE;
BIT ALUSrc    = FALSE;
BIT RegWrite  = FALSE;
BIT Zero      = FALSE;
BIT ALUControl[4] = {FALSE};
BIT JumpReg   = FALSE;

void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  } 
  printf("\n");  
  
  printf("Register: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  } 
  printf("\n");
}


/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
/*
BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}
*/

void decoder3(BIT* I, BIT EN, BIT* O)
{
  O[0] = and_gate3(not_gate(I[2]), not_gate(I[1]), not_gate(I[0]));
  O[1] = and_gate3(not_gate(I[2]), not_gate(I[1]), I[0]);
  O[2] = and_gate3(not_gate(I[2]), I[1], not_gate(I[0]));
  O[3] = and_gate3(not_gate(I[2]), I[1], I[0]);
  O[4] = and_gate3(I[2], not_gate(I[1]), not_gate(I[0]));
  O[5] = and_gate3(I[2], not_gate(I[1]), I[0]);
  O[6] = and_gate3(I[2], I[1], not_gate(I[0]));
  O[7] = and_gate3(I[2], I[1], I[0]);
  
  O[0] = and_gate(EN, O[0]);
  O[1] = and_gate(EN, O[1]);
  O[2] = and_gate(EN, O[2]);
  O[3] = and_gate(EN, O[3]);
  O[4] = and_gate(EN, O[4]);
  O[5] = and_gate(EN, O[5]);
  O[6] = and_gate(EN, O[6]);
  O[7] = and_gate(EN, O[7]);
  
  return;
}

void decoder5(BIT* I, BIT*  O)
{
   BIT EN[4] = {FALSE};
   decoder2(I[3], I[4], &EN[0], &EN[1], &EN[2], &EN[3]);
   decoder3(I, EN[3], &O[24]);
   decoder3(I, EN[2], &O[16]);
   decoder3(I, EN[1], &O[8]);
   decoder3(I, EN[0], &O[0]);
}

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)
{
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here
  //get read adress 0-4
  //decode those into instruction
  BIT t[5];
  t[0] = ReadAddress[0];
  t[1] = ReadAddress[1];
  t[2] = ReadAddress[2];
  t[3] = ReadAddress[3];
  t[4] = ReadAddress[4];
  BIT dummy[32] = {FALSE};
  decoder5(t, dummy);
  //Not sure if this is right but this is best guess

  for(int i = 0; i<32; i++){
    BIT hold = dummy[i];
    //when hold is 1, get the resultant address from big set
    for(int j = 0; j<32; j++){
      Instruction[j] = multiplexor2(hold, Instruction[j], MEM_Instruction[i][j]);
    }
  }
  
}

void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* JumpReg)
{
  // TODO: Set control bits for everything
  // Input: opcode field from the instruction
  // Output: all control lines get set 
  // Note: Can use SOP or similar approaches to determine bits

  ALUOp[0] = OpCode[2]; //beq
  ALUOp[1] = nor_gate(OpCode[5], OpCode[2]); //R-type

  RegDst[0] = not_gate(or_gate3(OpCode[3],OpCode[2],OpCode[1])); //TRUE if R-type instruction
  RegWrite[0] = or_gate(nor_gate(OpCode[2], OpCode[1]), and_gate(OpCode[5], not_gate(OpCode[3]))); //TRUE if register is being written (R, addi, lw)
  ALUSrc[0] = or_gate(OpCode[5], OpCode[3]); //TRUE if second input to ALU is immediate value (lw, sw, addi)
  MemRead[0] = and_gate(not_gate(OpCode[3]), OpCode[5]); //TRUE if reading from memory (lw)
  MemWrite[0] = and_gate(OpCode[3], OpCode[0]); //TRUE if writing to memory (sw)
  MemToReg[0] = and_gate(OpCode[5], OpCode[3]) ; //TRUE if writing to register from memory (lw)
  Branch[0] = OpCode[2]; //TRUE if branch instruction (beq)
  Jump[0] = and_gate(not_gate(OpCode[5]), OpCode[1]); //TRUE if jump instruction (j, jal)
  JumpReg[0] = and_gate3(not_gate(Branch[0]), not_gate(MemRead[0]), not_gate(RegWrite[0]));

}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit

  //dummy bit recieve
  BIT dummy1[32] = {FALSE};
  BIT dummy2[32] = {FALSE};
  decoder5(ReadRegister1, dummy1);
  decoder5(ReadRegister2, dummy2);
  
  for(int i = 0; i<32; i++){
    BIT hold1 = dummy1[i];
    BIT hold2 = dummy2[i];
    for(int j = 0; j<32; j++){
      ReadData1[j] = multiplexor2(hold1, ReadData1[j], MEM_Register[i][j]);
      ReadData2[j] = multiplexor2(hold2, ReadData2[j], MEM_Register[i][j]);
    }
  }
  
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
{
  // TODO: Implement register write functionality
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  // Note: Implementation will again be similar to those above

  //use dummy bit
  BIT dummy[32] = {FALSE};
  decoder5(WriteRegister, dummy);

  for(int i = 0; i<32; i++){
    BIT hold = dummy[i];
    for(int j = 0; j<32; j++){
      BIT kill = MEM_Register[i][j];
      MEM_Register[i][j] = multiplexor4(hold, RegWrite, kill, kill, kill, WriteData[j]);
    }
  }
  //only write if regwrite is true
  //same as before, writedata at the point where dummy is 1
  //use index in register file
  
}

void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  // TODO: Implement ALU Control circuit
  // Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
  //        binary instruction
  // Output:4-bit ALUControl for input into the ALU
  // Note: Can use SOP or similar approaches to determine bits
  
  ALUControl[0] = and_gate(ALUOp[1], or_gate(funct[0], funct[3])); //or, slt
  ALUControl[1] = nand_gate(ALUOp[1], funct[2]); //addi, lw, sw, beq, add, sub, slt
  ALUControl[2] = or_gate(ALUOp[0], and_gate(ALUOp[1], funct[1])); //beq, sub, slt
}

void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  
  BIT x0 = xor_gate(A, B);
  *Sum = xor_gate(CarryIn, x0);
  
  BIT y0 = and_gate(x0, CarryIn);
  BIT y1 = and_gate(A, B);
  *CarryOut = or_gate(y0, y1);
}

void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set)
{
  
  BIT x0 = multiplexor2(Binvert, B, not_gate(B));
  
  BIT y0 = and_gate(A, x0);
  BIT y1 = or_gate(A, x0);
  
  BIT y2 = FALSE;
  adder1(A, x0, CarryIn, CarryOut, &y2); 
  *Set = y2;
  
  BIT y3 = Less; 
  
  *Result = multiplexor4(Op0, Op1, y0, y1, y2, y3);
}

void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result)
{   
  // TODO: Implement 32-bit ALU
  // Input: 4-bit ALUControl, two 32-bit inputs
  // Output: 32-bit result, and zero flag big
  // Note: Can re-use prior implementations (but need new circuitry for zero)

  //make the 1 bit, figure out how to parse 4-bit op, imp 32 bit, make for or loop for zero.
  BIT Less = FALSE;
  BIT Set = FALSE;
  BIT CarryOut[1] = {FALSE};
//ALUControl bits swapped in at binver, Cin, op0, and op1
  ALU1(Input1[0], Input2[0], ALUControl[2], ALUControl[2], Less, 
    ALUControl[0], ALUControl[1], &Result[0], CarryOut, &Set);
  for (int i = 1; i < 32; ++i) {
    ALU1(Input1[i], Input2[i], ALUControl[2], *CarryOut, Less, 
      ALUControl[0], ALUControl[1], &Result[i], CarryOut, &Set);
  }
  
  Less = Set;
  ALU1(Input1[0], Input2[0], ALUControl[2], ALUControl[2], Less, 
    ALUControl[0], ALUControl[1], &Result[0], CarryOut, &Set);

//go through result checking for any TRUE bits, zero is not_gate of the solution
  BIT or11 = or_gate(Result[0], Result[1]);
  for(int i = 2; i<32; i++){
    or11 = or_gate(or11, Result[i]);
  }

  *Zero = not_gate(or11);

  
}

void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above

  //combine read/write
  //do both
  //if memwrite, write, if memread, read
  //use process to get address, then either write or read
  BIT dummy[32] = {FALSE};
  BIT t[5];
  t[0] = Address[0];
  t[1] = Address[1];
  t[2] = Address[2];
  t[3] = Address[3];
  t[4] = Address[4];

  decoder5(t, dummy);

  for(int i = 0; i<32; i++){
    BIT hold = dummy[i];
    for(int j = 0; j<32; j++){
      BIT kill = MEM_Data[i][j];
      MEM_Data[i][j] = multiplexor4(hold, MemWrite, kill, kill, kill, WriteData[j]);
      kill = ReadData[j];
      ReadData[j] = multiplexor4(hold, MemRead, kill, kill, kill, MEM_Data[i][j]);
    }
  }
  
}

void Extend_Sign16(BIT* Input, BIT* Output)
{
  // TODO: Implement 16-bit to 32-bit sign extender
  // Copy Input to Output, then extend 16th Input bit to 17-32 bits in Output
  //Fill in the output first 16 bits with the values
  for(int i = 0; i<16; i++){
    Output[i] = Input[i];
  }
//Fill in the last 16 bits with the last value of input
  for(int i = 16; i<32; i++){
    Output[i] = Input[15];
  }
  
}

void updateState()
{
  // TODO: Implement the full datapath here
  // Essentially, you'll be figuring out the order in which to process each of 
  // the sub-circuits comprising the entire processor circuit. It makes it 
  // easier to consider the pipelined version of the process, and handle things
  // in order of the pipeline. The stages and operations are:
  // Fetch - load instruction from instruction memory
  // Decode - set control bits and read from the register file
  // Execute - process ALU
  // Memory - read/write data memory
  // Write Back - write to the register file
  // Update PC - determine the final PC value for the next instruction

  //create an array for instructions
  BIT Instruction[32] = {FALSE};

  //let's begin with fetch - load instruction from memory
  Instruction_Memory(PC, Instruction);

  //set opcode for entry into control
  BIT OpCode[6];

  for(int i = 26; i<32; i++){
    OpCode[i-26] = Instruction[i];
  }

  //set control bits and read from register file
  Control(OpCode, &RegDst, &Jump, &Branch, &MemRead, &MemToReg, 
    ALUOp, &MemWrite, &ALUSrc, &RegWrite, &JumpReg);

  //create read registers
  BIT ReadRegister1[5];
  BIT ReadRegister2[5];
  BIT WriteRegister[5];
  BIT WriteRegister_premux[5];
  BIT ReadData1[32];
  BIT ReadData2[32];

  for(int i = 21; i<26; i++){
    ReadRegister1[i-21] = Instruction[i];
  }

  for(int i = 16; i<21; i++){
    ReadRegister2[i-16] = Instruction[i];
  }

  for(int i = 11; i<16; i++){
    WriteRegister_premux[i-11] = Instruction[i];
  }

  multiplexor2_32(RegDst, ReadRegister2, WriteRegister_premux, WriteRegister);

  //read registers
  Read_Register(ReadRegister1, ReadRegister2, ReadData1, ReadData2);

  //process ALU
  BIT funct[6] = {FALSE};

  //create 6 bit funct array
  for(int i = 0; i<6; i++){
    funct[i] = Instruction[i];
  }
  
  ALU_Control(ALUOp, funct, ALUControl);

  //prepare for ALU processing
  //instruction fetching and sign extension
  BIT pre_extended[16];
  BIT post_extended[32];
  for(int i = 0; i<16; i++){
    pre_extended[i] = Instruction[i];
  }

  Extend_Sign16(pre_extended, post_extended);

  //setting up pre-ALU mux
  BIT alu_input2[32];
  multiplexor2_32(ALUSrc, ReadData2, post_extended, alu_input2);

  //running ALU
  BIT Result[32];
  ALU(ALUControl, ReadData1, alu_input2, &Zero, Result);

  //read/write data memory
  //assuming ReadData is a 32 bit array
  BIT ReadData[32];
  Data_Memory(MemWrite, MemRead, Result, ReadData2, ReadData);

  //preparing to write
  BIT WriteData[32];
  multiplexor2_32(MemToReg, Result, ReadData, WriteData);
  //write to the register file
 
  Write_Register(RegWrite, WriteRegister, WriteData);

  //update PC -- add 1 to final pc value no need to shift left
  //add 1 using ALU

  BIT addco[] = {0,1,0,0};

  //copy array into temp array
  BIT temp_PC[32];
  for(int i = 0; i<32; i++){
    temp_PC[i] = PC[i];
  }

  ALU(addco, temp_PC, ONE, &Zero, PC);

  BIT jump_alu_result[32];
  ALU(addco, PC, post_extended, &Zero, jump_alu_result);

  BIT mux1[32];

  multiplexor2_32(and_gate(Zero, Branch), PC, jump_alu_result, mux1);

  //create sub-pc array
  /*
  BIT pc_31_28[4];
  for(int i = 28; i<32; i++){
    pc_31_28[i-28] = PC[i];
  }
  */
  //create jump address
  BIT jump_address[32] = {FALSE};
  for(int i = 0; i<26; i++){
    jump_address[i] = Instruction[i];
  }

  //update final pc value

  BIT mux2[32];
  multiplexor2_32(Jump, mux1, jump_address, mux2);

  multiplexor2_32(JumpReg, mux2, ReadData1, PC);

  
}


/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);
    
  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);
  
  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);
  
  while (binary_to_integer(PC) < counter) {
    print_instruction();
    updateState();
    print_state();
  }

  return 0;
}

