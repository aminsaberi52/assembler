#include<stdio.h>
#include<string.h>

char** lable = NULL;
int* Final_Address;
char matrix[100][100] = { 0 };
int second_counter = 0;

int lable_finder(char* offset) 
{	
	 // finds if the offset is in the lable list
	int c = 0;
	char word[6] = { 0 };
	while (c < second_counter) 
	{
		if (lable[c] == 9) 
		{
			c++;
			continue;
		}
		
		else 
		{
			//printf("\nthis is lable[i] :  %s", lable[i]);
			strcpy(word, lable[c]);
			//printf("%s", word);
			if (strcmp(offset, word) == 0)
				return c;
			c++;
		}
	}
	// if the offset wasn't define in lable list Exit(1) make error
	printf("\nThis lable : %s is not identified, Please try again later ... \n", offset);
	exit(1);
}

int R_type(char* registers, char* command) 
{
	int rd = 0, rs = 0, rt = 0, c = 0, final = 0, opcode = 0;

	while (registers[c] != 44 && registers[c] != 0) // untill null or kama
	{
		rd = rd * 10 + registers[c] - 48;
		c++;
	}
	
	c++;
	
	while (registers[c] != 44 && registers[c] != 0) // untill null or kama
	{
		rs = rs * 10 + registers[c] - 48;
		c++;
	}
	
	c++;
	
	while (registers[c] != 44 && registers[c] != ' ' && registers[c] != 0) // untill null or space or kama 
	{
		rt = rt * 10 + registers[c] - 48;
		c++;
	}

	if (strcmp(command, "add") == 0) 	
		opcode = 0;
	
	else if (strcmp(command, "sub") == 0) 
		opcode = 1;
	
	else if (strcmp(command, "slt") == 0) 
		opcode = 2;
	
	else if (strcmp(command, "or") == 0) 
		opcode = 3;
	
	else if (strcmp(command, "nand") == 0) 
		opcode = 4;
	

	rs = rs << 20;
	rt = rt << 16;
	rd = rd << 12;
	
	opcode = opcode << 24;

	final = final | rs;
	final = final | rt;
	final = final | rd;
	final = final | opcode;

	return final;
}

int I_type(char* registers, char* command, int index) 
{
	int rs = 0, rt = 0, opcode = 0, final = 0, offint = 0;
	char offset[7] = { 0 };

	// if instruction is addi, ori, slti
	if (strcmp(command, "addi") == 0 || strcmp(command, "ori") == 0 || strcmp(command, "slti") == 0) 
	{
		int imm = 0, c = 0;
		// finds register target
		while (registers[c] != 44 && registers[c] != 0) // untill null or kama
		{
			rt = rt * 10 + registers[c] - 48;
			c++;
		}
		
		c++;
		
		// finds register source
		while (registers[c] != 44 && registers[c] != 0) // untill null or kama
		{
			rs = rs * 10 + registers[c] - 48;
			c++;
		}
		
		c++;
		
		// finds immediate
		while (registers[c] != 44 && registers[c] != ' ' && registers[c] != 0) // untill null or kama or space
		{
			imm = imm * 10 + registers[c] - 48;
			c++;
		}

		// addi op=5
		if (strcmp(command, "addi") == 0) 
			opcode = 5;

		// slti op=6
		else if (strcmp(command, "slti") == 0) 
			opcode = 6;
		
		// ori op=7
		else if (strcmp(command, "ori") == 0) 
			opcode = 7;
		
		// if constant number is greater than 16 bit the exit(1)
		if (imm > 65535) 
		{
			printf("\nThis offset has more than 16 bits, Please try again later ... \n");
			exit(1); // exit(1)
		}
				
		final = final | imm;
	}
	
	else if (strcmp(command, "lw") == 0 || strcmp(command, "sw") == 0 )  
	{
		int c = 0;

		// finds register target
		while (registers[c] != 44 && registers[c] != 0) // untill null or kama
		{
			rt = rt * 10 + registers[c] - 48;
			c++;
		}
		
		c++;
		
		// finds register source
		while (registers[c] != 44 && registers[c] != 0) // untill null or kama
		{
			rs = rs * 10 + registers[c] - 48;
			c++;
		}
		
		c++;
		
		// checks if offset is number 
		if (registers[c] <= 57 && registers[c] >= 48) 
		{
			while (registers[c] != 44 && registers[c] != 0) // untill null or kama
			{
				offint = offint * 10 + registers[c] - 48;
				c++;
			}
			
			final = final | offint;
		}

		// if offset is string
		else 
		{
			int a = 0;
			
			while (registers[c] != 44 && registers[c] != 0 && registers[c] != 9 && registers[c] != 32) 
			{
				offset[a] = registers[c];
				a++;
				c++;
			}
			
			// checks if offset is in lable list
			int address = lable_finder(offset);

			// puts offset address in final
			final = final | address;
		}

		if (strcmp(command, "lw") == 0) 
			opcode = 9;		

		else if (strcmp(command, "sw") == 0) 
			opcode = 10;
	}
	
	else if (strcmp(command, "beq") == 0)
	{
		int c = 0;

		// finds register target
		while (registers[c] != 44 && registers[c] != 0) // untill null or kama
		{
			rt = rt * 10 + registers[c] - 48;
			c++;
		}
		
		c++;
		
		// finds register source
		while (registers[c] != 44 && registers[c] != 0) // untill null or kama
		{
			rs = rs * 10 + registers[c] - 48;
			c++;
		}
		
		c++;
		
		// the offset of beq is lable 
		int a = 0;
		while (registers[c] != 44 && registers[c] != 0 && registers[c] != 9 && registers[c] != 32) 
		{
			offset[a] = registers[c];
			a++;
			c++;
		}
		
		// checks if offset is in lable list
		int address = lable_finder(offset);
		address = address - index - 1;
		
		// puts offset address in final
		final = final | address;
		opcode = 11;
	}
	
	else if (strcmp(command, "jalr") == 0) 
	{
		int c = 0;
		// finds register target
		while (registers[c] != 44 && registers[c] != 0) // untill null or kama
		{
			rt = rt * 10 + registers[c] - 48;
			c++;
		}
		
		c++;
		
		// finds register source
		while (registers[c] != 44 && registers[c] != 0) // untill null or kama
		{
			rs = rs * 10 + registers[c] - 48;
			c++;
		}
		
		opcode = 12;
	}
	
	else if (strcmp(command, "lui") == 0) 
	{
		int c = 0, imm = 0;
		while (registers[c] != 44 && registers[c] != 0) // untill null or kama
		{
			rt = rt * 10 + registers[c] - 48;
			c++;
		}
		
		c++;
		
		// finds immediate
		while (registers[c] != 44 && registers[c] != ' ' && registers[c] != 0) // untill null or kama or space
		{
			imm = imm * 10 + registers[c] - 48;
			c++;
		}
		
		opcode = 8;
		if (imm > 65535) 
		{
			printf("\nThis offset has more than 16 bits, Please try again later ... \n");
			exit(1); // exit(1)
		}
		
		final = final | imm;
	}
	
	opcode = opcode << 24;
	rt = rt << 16;
	rs = rs << 20;
	final = final | opcode;
	final = final | rt;
	final = final | rs;

	return final;
}

int J_type(char* registers, char* command) 
{
	char offset[7] = "";
	int opcode = 0, final = 0;
	
	if (strcmp(command, "halt") == 0) 
		opcode = 14;
		
	else if (strcmp(command, "j") == 0) 
	{
		opcode = 13;
		strcpy(offset, registers);
		int address = lable_finder(offset);
		final = final | address;
	}
	
	opcode = opcode << 24;
	final = final | opcode;

	return final;
}

int director(char* registers, char* command) 
{
	if (registers[0] == 45) 
	{
		int neg = 0;
		int c = 1;
		
		while (registers[c] != 9 && registers[c] != 32 && registers[c] != 0) // untill null or kama
		{
			neg = neg * 10 + registers[c] - 48;
			c++;
		}
		
		neg *= -1;
		return neg;
	}
	
	else if (registers[0] >= 48 && registers[0] <= 57) 
	{
		int pos = 0;
		int c = 0;
		
		while (registers[c] != 9 && registers[c] != 32 && registers[c] != 0) // untill null or kama
		{
			pos = pos * 10 + registers[c] - 48;
			c++;
		}
		return pos;
	}
	
	else 
	{
		int address = lable_finder(registers);
		return address;
	}
}

int  matrix_maker() 
{
	char path[100] = "";
	printf("\nEnter your file's address : ");
	gets(path);
	
	//C:\\C\\test2.as
	// for command promp   C:\C\Project2AR\Project2AR>gcc -std=c99 mehran.c
	FILE* Myfile = fopen(path, "r");
	if (Myfile == NULL)
	{
		printf("Wrong file or path !!! Please try again later ... ");
		exit(1);
	}

	int y = 0;
	int c = 0;
	
	while (fgets(matrix[y], sizeof(matrix[y]), Myfile))
	{
		//strcpy(mat[j] , line);
		printf("%s", matrix[y]);
		int label = matrix[y];
		y++;
		c++;
	}
	
	fclose(Myfile);
	return c;
}

void lable_checker(char* word, int index) 
{
	for (int i = 0; i < index; i++) 
	{
		if (lable[i] == 9) 
			continue;
		
		else 
		{
			// if lable is dublicated
			if (strcmp(lable[i], word) == 0) 
			{
				printf("\nThe lable %s is dublicated, Please try again later ... ", word);
				exit(1);
			}
		}
	}
}

void lable_maker(int c) 
{
	lable = malloc(c * sizeof(char*));
	for (int i = 0; i < c; i++)	
		lable[i] = malloc(7 * sizeof(char));
	
	for (int x = 0; x < c; x++)
	{
		//lable[i] = "";
		if (matrix[x][0] != 32 && matrix[x][0] != 9)
		{
			char word[7] = "";
			int a = 0;
			for (int y = 0; y < 100; y++)
			{
				if (matrix[x][y] == 9 || matrix[x][y] == 32)
					break;
					
				else
				{
					word[a] = matrix[x][y];
					a++;
				}
			}
			
			lable_checker(word, x);
			strcpy(lable[x], word);
		}
		
		else 
			lable[x] = 9;
	}
}

void read_line(int size) 
{
	for (int i = 0; i < size; i++) 
	{
		// add,sub,slt,or,nand
		char command[7] = "";

		//1,2,3,4,5,6,7,8,9,10,...,16
		char registers[20] = { 0 };

		int x = 0; //index of command
		char line[100] = { 0 };
		strcpy(line, matrix[i]);

		int c = 0; // index of line

		while (line[c] >= 48) // skip label
			c++;

		while (line[c] == 32 || line[c] == 9) // skip tab
			c++;

		int c1 = c;
		for (int j = c1; j < c1 + 7; j++) 
		{
			if (line[j] >= 46) 
			{
				command[x] = line[j];
				x++;
				c++;
			}
			
			else 
				break;			
		}
		
		// skip tab until reach registers
		while (line[c] == 32 || line[c] == 9) 
			c++;		

		//c reached 1,2,3
		x = 0;
		while (line[c] >= 44) 
		{
			registers[x] = line[c];
			x++;
			c++;
		}
		
		//printf("\nthis is command %s \nand this is registers: %s", command, registers);
		//add <- command
		//1,2,3 <- registers
		if (strcmp(command, "add") == 0 || strcmp(command, "sub") == 0 || (strcmp(command, "slt") == 0) || (strcmp(command, "or") == 0) || (strcmp(command, "nand") == 0)) 
		{
			int address = R_type(registers, command);
			Final_Address[i] = address;
		}
		
		else if (strcmp(command, "addi") == 0 || strcmp(command, "ori") == 0 || (strcmp(command, "slti") == 0) || (strcmp(command, "lui") == 0) || (strcmp(command, "lw") == 0) || strcmp(command, "sw") == 0 || (strcmp(command, "jalr") == 0) || (strcmp(command, "beq") == 0)) 
		{
			int address = I_type(registers, command, i);
			Final_Address[i] = address;
		}
		
		else if (strcmp(command, "halt") == 0 || strcmp(command, "j") == 0) 
		{
			int address = J_type(registers, command);
			Final_Address[i] = address;
		}
		
		else if (strcmp(command, ".fill") == 0 || strcmp(command, ".space") == 0) 
		{
			int address = director(registers, command);
			Final_Address[i] = address;
		}
		
		else 
		{
			printf("\nThe opcode %s isn't valid, Please try again later ... ", command);
			exit(1);
		}		 
		//printf("\nthis is address of line %d  --> %d", i, final_address[i]);
	}
}

void save_output(int count) 
{
	char File_Name[30];
	printf("\nEnter your saving file's address and name : ");
	gets(File_Name);

	FILE* My_File = fopen(File_Name, "w");
	
	for (int c = 0; c < count; c++) 
	{
		int address = Final_Address[c];
		fprintf(My_File, "%d\n", address);
	}
	
	fclose(My_File);
}

int main() 
{
	int count = matrix_maker();
	
	second_counter = count;
	Final_Address = malloc(count * sizeof(int));
	lable_maker(count);
	read_line(count);
	
	for (int c = 0; c < count; c++)
		printf("\n%d", Final_Address[c]);
		
	save_output(count);
	
	return 0;	
}