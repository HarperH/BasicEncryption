//Encrypt a .txt document utilizing the associated key file
//Dev: Harper Hill

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define PASSLEN 256

int menu(void);
void encrypt(FILE *fin,FILE *fout,char subarr[],char pass[]);
void decrypt(FILE *fin,FILE *fout,char subarr[],char pass[]);
void readKeyFile(FILE *fin,char keyFileIdxChar[]);
void createCharIdxKeyFile(char keyFileIdxChar[], char keyFileCharIdx[]);


int main(void){
	int menuChoice = 0;
	char password[PASSLEN];
	char inputFileName[PASSLEN];
	char outputFileName[PASSLEN];
	char keyFileIdxChar[PASSLEN];
	char keyFileCharIdx[PASSLEN];
	FILE *inputFile;
	FILE *outputFile;
	FILE *keyFile;
	
	//get password from user
	printf("Welcome to your encryption program\nEnter your password: ");
	fgets(password,PASSLEN,stdin);
	password[strcspn(password,"\n")] = 0;
	
	//read keyfile into idx to char array and char to idx array
	keyFile = fopen("key.254", "r");
	if(keyFile == NULL){
		printf("failed to open keyfile");
	}
	readKeyFile(keyFile,keyFileIdxChar);
	createCharIdxKeyFile(keyFileIdxChar,keyFileCharIdx);
	
	//give menu options to user until they type quit
	while(menuChoice == 0){
		menuChoice = menu();
	
		if(menuChoice == 1){
			
			//get filename to be encrypted and connect to it
			printf("Enter the name of the file you'd like to encrypt: ");
			fgets(inputFileName,PASSLEN,stdin);
			inputFileName[strcspn(inputFileName,"\n")] = 0;
			inputFile = fopen(inputFileName, "r");
			
			//if connection is unsuccessful start over
			if(inputFile == NULL){
				printf("cannot open file %s",inputFileName);
				
			//if connection is successful get output filename and connect
			} else {
				printf("Enter the desired name of the output file: ");
				fgets(outputFileName,PASSLEN,stdin);
				outputFileName[strcspn(outputFileName,"\n")] = 0;
				outputFile = fopen(outputFileName, "w");
				
				//if connection is unsuccessful start over
				if(outputFile == NULL){
					printf("cannot create file %s",outputFileName);
				
				//if connection is successful run encryption function
				} else {
					encrypt(inputFile,outputFile,keyFileIdxChar,password);
					
					//add a little magic for the user
					for(int i = 0;i < 3;i++){
						printf(".");
						sleep(1);
					}
					printf("File encrypted\n\n");
				}
			}
			menuChoice = 0;
		} else if(menuChoice == 2){
			
			//get filename to be decrypted and connect to it
			printf("Enter the name of the file you'd like to decrypt: ");
			fgets(inputFileName,PASSLEN,stdin);
			inputFileName[strcspn(inputFileName,"\n")] = 0;
			inputFile = fopen(inputFileName, "r");
			
			//if connection is unsuccessful start over
			if(inputFile == NULL){
				printf("cannot open file %s",inputFileName);
				
			//if connection is successful get output filename and connect
			} else {
				printf("Enter the desired name of the output file: ");
				fgets(outputFileName,PASSLEN,stdin);
				outputFileName[strcspn(outputFileName,"\n")] = 0;
				outputFile = fopen(outputFileName, "w");
				
				//if connection is unsuccessful start over
				if(outputFile == NULL){
					printf("cannot create file %s",outputFileName);
				
				//if connection is successful run encryption function
				} else {
					decrypt(inputFile,outputFile,keyFileCharIdx,password);
					
					//add a little magic for the user
					for(int i = 0;i < 3;i++){
						printf(".");
						sleep(1);
					}
					printf("File decrypted\n\n");
				}
			}
			menuChoice = 0;
			
		//if menu choice is three close program
		} else if(menuChoice == 3){
			printf("Thank you for using our program");
			return(0);
		}
	}
}


int menu(void){
	char menuItem[PASSLEN];
	char firstChar;
	
	//get input from user
	printf("Please select an option below\n\n");
	printf("e:ncode\nd:ecode\nq:uit\n\nChoice: ");
	fgets(menuItem,PASSLEN,stdin);
	
	
	//select menu option based on first letter entered
	firstChar = tolower(menuItem[0]);
	
	if(firstChar == 'e'){
		return(1);
	} else if(firstChar == 'd'){
		return(2);
	} else if(firstChar == 'q'){
		return(3);
	} else {
		printf("please input e, d, or q\n");
		return(0);
	}
}


void readKeyFile(FILE *fin,char keyFileIdxChar[]){
	char buffer;
	
	//pull characters one at a time from keyfile and assign them to array until end of file
	for(int i=0; (buffer = fgetc(fin)) != EOF; i++){
		//printf("%d:%c:%c  \n",i,i,buffer);
		keyFileIdxChar[(unsigned char) i] = buffer;
	}
}


//create array mapping indexes to chars, opposite of the original array
void createCharIdxKeyFile(char keyFileIdxChar[], char keyFileCharIdx[]){
	for(int i = 0; i < 255; i++){
		keyFileCharIdx[(unsigned char)keyFileIdxChar[i]] = i;
	}
}


void encrypt(FILE *fin,FILE *fout,char subarr[],char pass[]){
	char ptChar;
	char preKey;
	char ctChar;
	int i = 0;
	
	//add char from pt doc to next character of password, then map that to keyfile and write resulting ct char to output file
	while((ptChar = fgetc(fin)) != EOF){
		preKey = ((unsigned char)ptChar + (unsigned char)pass[i%strlen(pass)])%255;
		ctChar = subarr[(unsigned char)preKey];
		//printf("%d:pt char:%c pass:%c preKey:%c ct char:%c\n",i,ptChar,(pass[i%strlen(pass)]%255),preKey,ctChar);
		fputc(ctChar, fout);
		i++;
	}
	
	//close connections when done
	fclose(fin);
	fclose(fout);
}


void decrypt(FILE *fin,FILE *fout,char subarr[],char pass[]){
	char ptChar;
	char preKey;
	char ctChar;
	int i = 0;
	
	//map ct char to keyfile then subtract next password character and write resulting pt char to output file
	while((ctChar = fgetc(fin)) != EOF){
		preKey = subarr[(unsigned char)ctChar];
		ptChar = ((unsigned char)preKey - (unsigned char)pass[i%strlen(pass)])%255;
		//printf("%d:ct char:%c preKey:%c pass:%c pt char:%c\n",i,ctChar, preKey, pass[i%strlen(pass)],ptChar);
		fputc(ptChar, fout);
		i++;
	}
	
	//close connections
	fclose(fin);
	fclose(fout);
}