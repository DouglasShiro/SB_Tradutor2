#include "montador.hpp"

int main(int argc, char **argv)
{

	int tamanho,  i = 0;
	locale loc;
	ifstream inFile;
	ofstream outFile;

	string  in(argv[1]);
	string	out(argv[2]);
	string line, str, aux;
	/*
		ESTRUTURA DE DADOS
	*/
	vector<string> arq, token;
	map<string,int> simbTable;
	unordered_map<string,int> defTable;
	unordered_multimap<string,int> useTable;
	unordered_multimap<string,int> reverso;
	map<string,string> opTable;
	map<string,int> dirTable;
	vector<string> code;				/*vetor que ira receber os codigo objeto a ser salvo em arquivo*/
	vector<int> relativo;
	/*
		ITERADORES
	*/
	map<string,int>::iterator it;		/*iterador para tabelas formato string, int*/
	map<string,string>::iterator itOp;	/*iterador para tabelas formato string, string*/
	unordered_map<string,int>::iterator itMod;
	unordered_multimap<string,int>::iterator itUse;


	/*
		TABELA DE INSTRUCOES
	*/
	opTable["ADD"] = "01";
	opTable["SUB"] = "02";
	opTable["MULT"] = "03";
	opTable["DIV"] = "04";
	opTable["JMP"] = "05";
	opTable["JMPN"] = "06";
	opTable["JMPP"] = "07";
	opTable["JMPZ"] = "08";
	opTable["COPY"] = "09";
	opTable["LOAD"] = "10";
	opTable["STORE"] = "11";
	opTable["INPUT"] = "12";
	opTable["OUTPUT"] = "13";
	opTable["STOP"] = "14";
	/*
		NOVAS INSTRUCOES
	*/
	opTable["C_INPUT"] 	= "15";
	opTable["C_OUTPUT"] = "16";
	opTable["S_INPUT"] 	= "17";
	opTable["S_OUTPUT"] = "18";

	/*
		TABELA DE DIRETIVAS
	*/
	dirTable["SECTION"] = 1;
	dirTable["SPACE"] 	= 1;
	dirTable["CONST"] 	= 1;
	dirTable["EQU"] 	= 1;
	dirTable["IF"] 		= 1;


	if (argc != 3)
	{
		printf("Erro: numero invalido de argumentos\n");
	}
	else
	{
		//in = in + ".asm";
		inFile.open(in.c_str());	// Abre o arquivo
		/*  Insere cada linha no vetor string arq */
		while(getline(inFile,line)){
			arq.push_back(line);
			i++; /* numero de linhas */
		}
		inFile.close();

		tamanho = i; /* tamanho = numero de linhas */
		for(i = 0; i < tamanho ;i++){
			str = arq[i]; /* atribuo a str a string arq[i]- linha inteira do arquivo */
			istringstream ss(str);
			aux = ""; /* Zera a string auxiliar*/

			/* Separa as linhas em tokens tendo espaco como delimitadores*/
			while(ss >> aux){
				transform(aux.begin(), aux.end(), aux.begin(), ::toupper);
				/* Caso seja um comnetario */
				if(aux[0] == ';'){
					break;
				}
				else{
					token.push_back(aux);
				}
			}
			token.push_back("\n");
		}

	/* Programa ja foi pre processado */
	/* Realiza a primeira passagem e verifica alguns erros de diretivas e definicoes */
	primeira_passagem(opTable, dirTable,  token, simbTable, defTable, useTable);

	/* Realiza a segunda passagem e cria o arquivo de saida */
	segunda_passagem(opTable, dirTable,  token, simbTable, defTable, useTable, code, relativo);

	// imprime_ia32();
	/* Printa o programa com as alteracoes */
		for(vector<string>::iterator it = token.begin(); it != token.end(); it++)
	 	cout << *it << " ";

		cout << "\nSIMBOL TABLE\n";
		for (it=simbTable.begin(); it != simbTable.end(); ++it)
			cout << it->first << "\t" << it->second<<"\n";
		cout << "\n";

		cout << "\nTABLE USE\n";
		reverso.insert(useTable.begin(), useTable.end());
		for (itUse=reverso.begin(); itUse!=reverso.end(); ++itUse)
			cout << itUse->first << "\t" << itUse->second<<"\n";
		cout << "\n";

		cout << "TABLE DEFINITION\n";
		for (itMod=defTable.begin(); itMod!=defTable.end(); ++itMod)
			cout << itMod->first << "\t" << itMod->second<<"\n";
		cout << "\n";

		cout << "RELATIVE\n";
		for(vector<int>::iterator it = relativo.begin(); it != relativo.end(); it++)
			cout << *it << " ";
		cout << "\n";
		cout<< "\n";

		cout << "CODE\n";
		for(vector<string>::iterator it = code.begin(); it != code.end(); it++)
			cout << *it << " ";
		cout << "\n";
		cout << "\n";

	}
	/* Caso nao tenha erros gera o codigo objeto */
	if(_erro == FALSE){
		//out = out + ".o";
		out = out + ".s";
		outFile.open(out, fstream::out);	// Abre o arquivo

		for(vector<string>::iterator it = code.begin(); it != code.end(); it++)
				outFile << *it << " ";

		/*FECHA ARQUIVO SAIDA*/
		outFile.close();
	}
	return 0;
}

/*
Função: Verificar se o token nao possui caracteres invalidos
input: token;
output: boolean;
*/
int token_valido(string token)
{
	int i = 0;
	/* Se o primeiro caractere eh um digito -> ERRO*/
	if(isdigit(token[0]))
	{
		return false;
	}
	while(token[i] != '\0')
	{		/* Se o nao e numero, letra ou underscore -> ERRO*/
			if(!isalnum(token[i]) && !(token[i] == '_'))
			{
					return false;
			}
		i++;
	}

return true;
}

/*
Função: primeira passagem do algoritmo de duas passagens,
 		gerar tabelas de simbolos, definição e uso,
		Verificação de erros;
input:	map<string,string> 					&opTable,   //Tabela Operações
		map<string,int> 					&dirTable,	//Tabela Diretivas
		map<string,int> 					&simbTable,	//Tabela de Síbolos
		unordered_map<string, int> 			&defTable,	//Tabela de Definição
		unordered_multimap<string, int> 	&useTable	//Tabela de Uso
		vector<string> 						&token,		//Token
output:	Tabelas simbolos, defiinição e suo preenchidas
*/
int primeira_passagem(map<string,string>& opTable, map<string,int>& dirTable,
						vector<string>& token,	map<string,int>& simbTable,
						unordered_map<string,int>& defTable, unordered_multimap<string,int>& useTable){

	char c;
	int existe = 0,  posCount = 0, lineCount = 1, temData = FALSE;
	string rotulo, str, valor;
	map<string,int>::iterator it;
	map<string,string>::iterator itOp;
	unordered_map<string,int>::iterator itMod;
	unordered_multimap<string,int>::iterator itUse;
	vector<string>::iterator itVec;

	for(string::size_type j = 0; j < token.size(); ++j){

		/* Verifica se eh um rotulo */
		if(token[j].find(":") != string::npos){
			/* retira os : do rotulo */
			rotulo = token[j].substr(0, token[j].length()-1);
			/* verifica se o token anterior era um rotulo */
			if(existe == 1){

				if(token[j+1].compare("CONST") || token[j+1].compare("SPACE")){
					cout << "ERRO SINTATICO:" << lineCount << ": Duas declarações na mesma linha\n";
					_erro = TRUE;
				}
				else{
					cout << "ERRO SINTATICO:" << lineCount << ": Dois rótulos na mesma linha\n";
					_erro = TRUE;
				}
			}

			if(token_valido(rotulo)){
				/* Verifica se o rotulo ja foi definido */
				it = simbTable.find(rotulo);
				if(it != simbTable.end()){
					if(!token[j+1].compare("CONST") || !token[j+1].compare("SPACE")){
						cout << "ERRO SEMANTICO:" << lineCount << ": Declaração repetida\n";
						_erro = TRUE;
					}
					else{
						cout << "ERRO SEMANTICO:" << lineCount << ": Rótulo repetido\n";
						_erro = TRUE;
					}				}
				else if(existe == 0){
					simbTable[rotulo] = posCount;
				}

				existe = 1;

			}
			else{

				cout << "ERRO LEXICO:" << lineCount << ": Token inválido\n";
				_erro = TRUE;
			}


		}
		/* Verifica se eh um operacao */
		else if((itOp = opTable.find(token[j])) != opTable.end()){
		/* Verifica se a instrucao esta na secao TEXT*/

			if((_sectionText > posCount) && (_sectionText != -1))
			{
				cout << "ERRO SEMANTICO:" << lineCount << ": A operação está fora da seção de texto \n";
				_erro = TRUE;

			}
			if ((_sectionData <= posCount) && (_sectionData != -1))	{

				cout << "ERRO SEMANTICO:" << lineCount << ": A operação está fora da seção de texto \n";
				_erro = TRUE;
			}

			/* Caso seja COPY-> tam 3*/
			if(!itOp->first.compare("COPY")){
				posCount += 3;
			}/* Caso seja STOP -> tam 1*/
			else if(!itOp->first.compare("STOP")){
				posCount += 1;
			} /* Todas as outras tam = 2*/
			else{
				posCount += 2;
			}
			existe = 0;

		}
		/* Verifica se eh uma diretiva */
		else if((it = dirTable.find(token[j])) != dirTable.end()){


			/* Diretiva SPACE */
			if(!it->first.compare("SPACE")){
				str = token[++j];
				int space = 0;
				temData = TRUE;

				/* Verifica de a instrucao esta na secao DATA */
				if((_sectionData < _sectionText) || (_sectionData > posCount) || (_sectionData == -1)){
					cout << "ERRO SEMANTICO:" << lineCount << ": A diretiva " << it->first << " está fora da seção de dados\n";
					_erro = TRUE;
				}

				/* Verifica se o operando eh nulo */
				if(!str.compare("\n"))
				{
					cout << "ERRO SINTATICO:"<<lineCount <<": Operando inválido\n";
					_erro = TRUE;
				}
				else{
					for (string::size_type i = 0; i < str.length(); ++i)
					{
						c=str[i];

						if(!isdigit(c)){

							cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido\n";
							_erro = TRUE;
							i = str.length();
						}else{
							space = space *10 + (c -'0');
						}

					}
					posCount+= space;

				}

			}
			/* Diretiva CONST */
			else if(!it->first.compare("CONST")){
				str = token[++j];
				temData = TRUE;
				string::size_type i = 0;

				/* Verifica de a instrucao esta na secao DATA */
				if((_sectionData < _sectionText) || (_sectionData > posCount) || (_sectionData == -1)){
						cout << "ERRO SEMANTICO:" << lineCount << ": A diretiva " << it->first << " está fora da seção de dados\n";
						_erro = TRUE;
				}

				/* Verifica se eh hexadecimal*/

				/*	cout << "\nSTRING => |" + str +"|\n";
				if(b  cvx!str.compare("0X0")){
					i = 2;
					cout << "\né sim->"+ str << endl;
				}else if (!str.compare(0,3,"-0X")){
					i = 3;
				}

				for (; i < str.length(); ++i)
				{
					c = str[i];
					if(!isdigit(c) && c != '-'){

						cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido\n";
						_erro = TRUE;
						i = str.length();
					}

				}*/

				posCount+= 1;
			}
			/* Diretiva EQU */
			else if(!it->first.compare("EQU")){
				valor = "";
				valor = token[j+1];
				/* Verifica se existe um rotulo antes do EQU */
				 if(!token[j-1].compare("\n"))
				 {
					cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido\n";
					_erro = TRUE;
				 }
				 else
				 {
					 rotulo = token[j-1].substr(0, token[j-1].length()-1);
					 /* Procura o rotulo na tabela de rotulos */
					if((it = simbTable.find(rotulo)) != simbTable.end()){
						/* Apaga o simbolo da tabela e substitui o rotulo pelo valor */
						simbTable.erase(it);
						if(find(token.begin(), token.end(), rotulo) != token.end())
						{
						 replace(token.begin(), token.end(), rotulo, valor);
						}
					}
					else{
						cout << "ERRO SEMANTICO:" << lineCount << ": Declaração ausente\n";
						_erro = TRUE;
					}
				}
			}
			/* Diretiva SECTION */
			else if(!it->first.compare("SECTION")){
				string operando = token [++j];
				/* Verifica se o operando eh TEXT */
				if(!operando.compare("TEXT")){
					_sectionText = posCount;
				}
				/* Verifica se o operando eh DATA */
				else if(!operando.compare("DATA")){
					_sectionData = posCount;
				} /* Senao erro */
				else{
					cout << "ERRO SINTATICO:"<< lineCount <<": Operando inválido para SECTION, deve ser TEXT ou DATA\n";
					_erro = TRUE;
				}

			}
			/* Diretiva IF */
			else if(!it->first.compare("IF")){
				string valor = "";
				valor = token[j+1];

				/* Se o rotulo existe na tabela de simbolos */
				if( simbTable.find(valor) != simbTable.end()){

					cout << "ERRO SINTATICO:" << lineCount << ": IF possui argumento inválido: \"" << valor << "\"\n";
					_erro = TRUE;
				}
				else if(!valor.compare("\n")){

					cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido\n";
					_erro = TRUE;
				}
				string::size_type n = j;
				while (token[n] != "\n")
				{
					token[n].erase();
					cout << "posCount: " << posCount << "	Token apagado: " << token[n] << "\n";
					n++;
				}
				if(valor.compare("1"))
				{

					n++;
					while (token[n] != "\n")
					{
						cout << "posCount: " << posCount << "	Token apagado: " << token[n] << "\n";
						token[n].erase();
						n++;
					}

				}

			}

			existe = 0;
		}
		str = "";
		/* Conta as linhas do programa */
		if(token[j] == "\n"){
			lineCount++;
		}
	}

	/* Verifica se a SECTION TEXT existe */
	if(_sectionText == -1)
	{
		cout << "ERRO SINTATICO: SECTION TEXT faltante \n";
					_erro = TRUE;
	}

	/* Verifica se a secao de dados existe */
	if((_sectionData == -1) && (temData == TRUE))
	{
		cout << "ERRO SINTATICO: SECTION DATA faltante \n";
		_erro = TRUE;
	}

	/* Verifica se a SECTION TEXT esta antes da SECTION DATA */
	if((_sectionData != -1) && (_sectionData < _sectionText))
	{
		cout << "ERRO SINTATICO: SECTION TEXT deve vir antes da SECTION DATA \n";
		_erro = TRUE;
	}

	return 0;
}

/*
Função:	Segunda passagem do algoritmo de duas passagens,

input:	map<string,string> 				&opTable,		//Tabela de Operaçôes
		map<string,int> 				&dirTable,		//Tabela de Diretivas
		map<string,int> 				&simbTable,		//Tabela de Simbolos
		unordered_map<string,int> 		&defTable,		//Tabela de Definições
		unordered_multimap<string,int> 	&useTable,		//Tabela de uso
		vector<string> 					&token,			//Token
		vector<string>					&code,			//Sessão de código a ser gravado no arquivo de saída
		vector<int>						&relativo		//Endereços relativos
output: Correção endereços relativos,
		Codigo do arquivo de saída,
		Tabela global de definições,

 */
int segunda_passagem(map<string,string> &opTable, map<string,int> &dirTable,
						vector<string> &token,	map<string,int> &simbTable,
						unordered_map<string,int> &defTable, unordered_multimap<string,int> &useTable,
						vector<string>&code, vector<int>&relativo){

		string memOp, memOp2; 				/*Operandos para operacoes*/
		string str;							/*string auxiliar*/
		string end;							/*string auxiliar para salvar endereco das operacoes e operandos*/
		string opCode;						/*string auxiliar para salvar o op code das operacoes*/
		char c;								/*char auxiliar*/
		int posCount = 0, lineCount = 1;
		int space = 0, posArray = 0, posArray2 = 0;						/*int para receber valor da diretiva SPACE*/
		int auxCode;
		map<string,int>::iterator it;		/*iterador para tabelas formato string, int*/
		map<string,string>::iterator itOp;	/*iterador para tabelas formato string, string*/
		unordered_map<string,int>::iterator itMod;
		unordered_multimap<string,int>::iterator itUse;

		/*percorre arquivo fonte*/
    for(string::size_type j= 0; j < token.size(); ++j){
        if(token[j].find(":") != string::npos){
			/* Eh um rotulo da section data */
			if(!token[j+1].compare("CONST") || !token[j+1].compare("SPACE")){
				string rotulo = token[j].substr(0, token[j].length()-1);
				code.push_back("\n" + rotulo);
			}else{ /* Eh um rotulo da section text */
				code.push_back("\n" + token[j]);
			}
        }else if((itOp = opTable.find(token[j])) != opTable.end()){
            /*
				OPERACOES
			*/
			if(!itOp->first.compare("STOP")){
				/*
					OPERAÇÃO: STOP
				*/
				/*se operacao for STOP verifica proximo token para ver se STOP nao possui operandos*/
				if((it = simbTable.find(token[j+1])) == simbTable.end()){
					//*se tudo der certo sexta tem mais um show*/
					posCount += 1;
					/*
						ARQUIVO SAIDA: STOP
					*/
					//code.push_back(itOp->second);
					code.push_back("\n\tmov eax, 1");
					code.push_back("\n\tmov ebx, 0");
					code.push_back("\n\tint 80h");

				}else{
					cout << "ERRO SINTATICO:" << lineCount << ": Formato inválido: STOP \n";
				}

			}
			else{ /* Caso seja outra operacao possui operando */
				posArray = 0;
				posArray2 = 0;

				memOp = token[j+1];
				str = memOp + "#";/* Verifica o EXTERN*/
				/* Verifica se possui um operando */
				if(!memOp.compare("\n"))
				{
					_erro = TRUE;
					cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido \n";

				} /* Verifica se o operando existe na tabela de simbolos */
				else if((simbTable.find(str) == simbTable.end()) &&
						(simbTable.find(memOp) == simbTable.end()) && itOp->first.compare("COPY"))
				{
					_erro = TRUE;
					//cout << "memOP >> "<< memOp <<"\n";
					if(!itOp->first.compare("JMP") || !itOp->first.compare("JMPN") ||
						!itOp->first.compare("JMPP") || !itOp->first.compare("JMPZ")){
						cout << "ERRO SEMANTICO:" << lineCount << ": Rótulo ausente \n";
					}
					else{
						cout << "ERRO SEMANTICO:" << lineCount << ": Declaração ausente \n";
					}

				}

				else{/* se ele existe e eh um operando valido */
					/* Verifica se eh um vetor */
					if(token[j+2] == "+"){
						if(token[j+3] != "\n"){ /* Verifica se a posicao do vetor nao eh nula */
							/* Verifica se eh um operando do COPY */
							if(token[j+3].find(",") != string::npos){
								posArray = stoi(token[j+3].substr(0, token[j+3].length()-1));
							}else{
									posArray = stoi(token[j+3]);
							}
							//cout <<"TOKEN: "<<token[j+3] << "	posArray:" << posArray <<"\n";
							/* Pega a posicao do operando na tabela de simbolos */
							if((it = simbTable.find(memOp)) != simbTable.end()){
								posArray += it->second;
								/* Verifica se a posicao eh valida consultando se exite alguma
								 * posicao maior que a posicao do memOp e menor que a posArray*/
								for(map<string,int>::iterator itArray = simbTable.begin(); itArray != simbTable.end(); itArray++){
									if((itArray->second > it->second) && (itArray->second <= posArray))
									{
										_erro = TRUE;
										cout << "ERRO SEMANTICO:" << lineCount << ": Endereço de memória não reservado \n";
										itArray = simbTable.end();
									}
								}
								posArray = stoi(token[j+3]);
							}
						}
						else{
							_erro = TRUE;
							cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido\n";
						}

					}

				/*
					OPERACAO: COPY
				*/
				if(!itOp->first.compare("COPY")){
					memOp = token[j+1]; /*Operando 1*/

					/* Verifica se o primeiro eh um array */
					if(token[j+2] == "+"){

						/* Verifica se o operando 1 eh valido */

						if((it = simbTable.find(memOp)) != simbTable.end()){
							if(it->second < _sectionData)
							{
								cout << "ERRO SEMANTICO:" << lineCount << ": Tipo de argumento inválido \n";

							}

							//end = posCount + 1;
							auxCode = it->second + posArray;
							//cout <<"AUXCODE: "<< auxCode	<< "\n"	;

						}
						else{
							_erro = TRUE;
							cout << "ERRO SEMANTICO:" << lineCount << ": Declaração ausente \n";
						}

						memOp2 = token[j+4];
						/* verifica se o operando 2 eh um array */
						if(token[j+5] == "+"){
							if(token[j+6] != "\n"){ /* Verifica se a posicao do vetor nao eh nula */
								posArray2 = stoi(token[j+6]);
								//cout <<"TOKEN: "<<token[j+6] << "	posArray2: " << posArray2 <<"\n";
								/* Pega a posicao do operando na tabela de simbolos */
									/* Verifica se a posicao eh valida consultando se existe alguma
									 * posicao maior que a posicao do memOp e menor que a posArray*/
									if((it = simbTable.find(memOp2)) != simbTable.end()){
										if(it->second < _sectionData)
										{
											cout << "ERRO SEMANTICO:" << lineCount << ": Tipo de argumento inválido \n";

										}
										posArray2 += it->second;
										for(map<string,int>::iterator itArray = simbTable.begin(); itArray != simbTable.end(); itArray++){
											if((itArray->second > it->second) && (itArray->second <= posArray))
											{
												_erro = TRUE;
												cout << "ERRO SEMANTICO:" << lineCount << ": Endereço de memória não reservado \n";
												itArray = simbTable.end();
											}
										}
										posArray2 = stoi(token[j+6]);
										/*se tudo der certo sexta tem mais um show*/
										/*ARQUIVO SAIDA*/
										/*salva OP CODE e end dos simbolos dos operandos de COPY*/
										/*
											ARQUIVO SAIDA: COPY ARRAY, ARRAY
											mov eax, [LABEL+ESI*2]
											mov [LABEL2+ESI*2], eax
										*/
										code.push_back(itOp->second);
										//code.push_back(to_string(auxCode + posArray));
										string auxiliar = "[" + it->first + "],";
										//code.push_back(to_string(it->second + posArray2));
										auxiliar = "[" + it->first + "], "; //Verificar qual it pegar
										code.push_back("\n\tmov\t");
										code.push_back(auxiliar);
										code.push_back("eax");

										relativo.push_back(posCount+1);
										relativo.push_back(posCount+2);

										posCount += 3;

								}
							}
							else{
								_erro = TRUE;
								cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido\n";
							}

						}/* Segundo operando nao eh um array*/
						else{
							if(memOp2.find(",") != string::npos){
								_erro = TRUE;
								cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido\n";
							}else{
								if((it = simbTable.find(memOp2)) != simbTable.end()){
									if(it->second < _sectionData)
									{
										cout << "ERRO SEMANTICO:" << lineCount << ": Tipo de argumento inválido \n";

									}

									/*se tudo der certo sexta tem mais um show*/
									/*ARQUIVO SAIDA*/
									/*salva OP CODE e end dos simbolos dos operandos de COPY*/
									/*
										ARQUIVO SAIDA: COPY ARRAY, MEM[OP2]
										mov eax, [LABEL]
										mov [LABEL2+ESI*2], eax
									*/
									code.push_back(itOp->second);
									code.push_back(to_string(auxCode));
									code.push_back(to_string(it->second));
									relativo.push_back(posCount+1);
									relativo.push_back(posCount+2);

									posCount += 3;

								}else{
									_erro = TRUE;
									cout << "ERRO SEMANTICO:" << lineCount << ": Declaração ausente\n";
								}
							}
						}
					}/* SE o primeiro operando nao eh um array */
					else{
						/* Verifica se o operando 1 eh valido */
						if(memOp.find(",") != string::npos){
							memOp = memOp.substr(0, memOp.length()-1);
							if((it = simbTable.find(memOp)) != simbTable.end()){
								if(it->second < _sectionData)
								{
									cout << "ERRO SEMANTICO:" << lineCount << ": Tipo de argumento inválido \n";

								}

								//end = posCount + 1;
								auxCode = it->second + posArray;
								//cout <<"AUXCODE: "<< auxCode	<< "\n"	;

							}
							else{
								_erro = TRUE;
								cout << "ERRO SEMANTICO:" << lineCount << ": Declaração ausente \n";
							}
						}
						else{
							_erro = TRUE;
							cout << "ERRO SINTATICO:" << lineCount << ": formato invalido: COPY A, B -> aqui miga\n";
						}

	                    memOp2 = token[j+2]; /*Operando 2*/
						if(token[j+3] == "+")
						{
							if(token[j+4] != "\n"){ /* Verifica se a posicao do vetor nao eh nula */
								posArray2 = stoi(token[j+4]);
								//cout <<"TOKEN: "<<token[j+4] << "posArray2: " << posArray2 <<"\n";
								/* Pega a posicao do operando na tabela de simbolos */
									/* Verifica se a posicao eh valida consultando se existe alguma
									 * posicao maior que a posicao do memOp e menor que a posArray*/
									if((it = simbTable.find(memOp2)) != simbTable.end()){
										if(it->second < _sectionData)
										{
											cout << "ERRO SEMANTICO:" << lineCount << ": Tipo de argumento inválido \n";

										}

										posArray2 += it->second;
										for(map<string,int>::iterator itArray = simbTable.begin(); itArray != simbTable.end(); itArray++){
											if((itArray->second > it->second) && (itArray->second <= posArray))
											{
												_erro = TRUE;
												cout << "ERRO SEMANTICO:" << lineCount << ": Endereço de memória não reservado \n";
												itArray = simbTable.end();
											}
										}
										posArray2 = stoi(token[j+4]);
										/*salva OP CODE e end dos simbolos dos operandos de COPY*/
										/*
											ARQUIVO SAIDA: COPY MEM[OP1], ARRAY
											mov eax, [LABEL2+ESI*2]
											mov [LABEL], eax
										*/
										code.push_back(itOp->second);
										string auxiliar = "[" + it->first + "]";
										code.push_back(auxiliar);
										//code.push_back(to_string(auxCode + posArray));
										//code.push_back(to_string(it->second + posArray2));
										relativo.push_back(posCount+1);
										relativo.push_back(posCount+2);

										posCount += 3;

								}
							}
							else{
								_erro = TRUE;
								cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido\n";
							}


						}else{/* Nenhum dos operando eh array*/

							if(memOp2.find(",") != string::npos){
								_erro = TRUE;
								cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido\n";
							}else{
								if((it = simbTable.find(memOp2)) != simbTable.end()){
									if(it->second < _sectionData)
									{
										cout << "ERRO SEMANTICO:" << lineCount << ": Tipo de argumento inválido \n";

									}

									/*ARQUIVO SAIDA*/
									/*salva OP CODE e end dos simbolos dos operandos de COPY*/
									/*
										ARQUIVO SAIDA: COPY MEM[OP1], MEM[OP2]
										mov eax, [LABEL]
										mov [LABEL2], eax
									*/
									code.push_back("\n\tmov\t");
									//code.push_back(itOp->second);
									string auxiliar = "[" + it->first + "], ";
									code.push_back(auxiliar);
									//code.push_back(to_string(auxCode));
									//code.push_back(to_string(it->second));
									relativo.push_back(posCount+1);
									relativo.push_back(posCount+2);
									code.push_back("eax");
									posCount += 3;

								}else{
									_erro = TRUE;
									cout << "ERRO SEMANTICO:" << lineCount << ": Declaração ausente\n";
								}
							}
						}

					}

				}
				else if(!itOp->first.compare("JMP") || !itOp->first.compare("JMPN") ||
					!itOp->first.compare("JMPP") || !itOp->first.compare("JMPZ")){
					memOp = token[j+1]; /* Operando 1 */
					if((it = simbTable.find(memOp)) != simbTable.end()){
						if(it->second >= _sectionData){
							_erro = TRUE;
							cout << "ERRO SEMANTICO:" << lineCount << ": Tipo de argumento inválido \n";
						}
						/*
							ARQUIVO SAIDA: JMP'S
						*/
						code.push_back(itOp->second);
						//code.push_back(to_string(it->second));
						code.push_back(it->first);
						relativo.push_back(posCount+1);

						posCount += 2;
					}
				}
				 else {
	                /*se operacao nao for COPY ou STOP verifica o proximo token
	                  e verifica se eh operando*/
					if((itOp = opTable.find(token[j])) != opTable.end()){
						/*Caso o proximo token seja um simbolo entao a operacao esta com os operandos corretos*/
						if((it = simbTable.find(memOp)) != simbTable.end()){
							/*ARQUIVO SAIDA*/
							if(it->second < _sectionData){
								_erro = TRUE;
								cout << "ERRO SEMANTICO:" << lineCount << ": Tipo de argumento inválido \n";
							}

							if(!itOp->first.compare("ADD")){
								/*
									ARQUIVO SAIDA:  ADD [CONST]/[SPACE 1]
								*/
								code.push_back("\n\tmov	esi, " + to_string(posArray));
								code.push_back("\n\tadd	eax, [" + it->first + " + ESI*2]");
								code.push_back("\n\tadd	eax, [" + it->first + "]");
								cout << "PosCount:" << posCount << "	OP: " << itOp->first << "\n";
								relativo.push_back(posCount+1);
							}else if(!itOp->first.compare("SUB")){
								/*
									ARQUIVO SAIDA:	SUB [CONST]/[SPACE 1]
								*/
								code.push_back("\n\tmov	esi, " + to_string(posArray));
								code.push_back("\n\tsub	eax, [" + it->first + " + ESI*2]");
								cout << "PosCount:" << posCount << "	OP: " << itOp->first << "\n";
								relativo.push_back(posCount+1);
							}else if(!itOp->first.compare("MULT")){
								/*
									ARQUIVO SAIDA:  MULT [CONST]/[SPACE 1]
								*/
								code.push_back("\n\tmov	esi, " + to_string(posArray));
								code.push_back("\n\tmul	[" + it->first + " + ESI*2]");
								cout << "PosCount:" << posCount << "	OP: " << itOp->first << "\n";
								relativo.push_back(posCount+1);
							}else if(!itOp->first.compare("DIV")){
								/*
									ARQUIVO SAIDA:  DIV [CONST]/[SPACE 1]
								*/
								code.push_back("\n\tmov	esi, " + to_string(posArray));
								code.push_back("\n\tdiv	[" + it->first + " + ESI*2]");
								cout << "PosCount:" << posCount << "	OP: " << itOp->first << "\n";
								relativo.push_back(posCount+1);
							}else if(!itOp->first.compare("LOAD")){
								/*
									ARQUIVO SAIDA:  LOAD
								*/
								code.push_back("\n\tmov	esi, " + to_string(posArray));
								code.push_back("\n\tmov	eax, [" + it->first + " + ESI*2]");
								cout << "PosCount:" << posCount << "	OP: " << itOp->first << "\n";
								relativo.push_back(posCount+1);
							}else if(!itOp->first.compare("STORE")){
								/*
									ARQUIVO SAIDA:  STORE
								*/
								code.push_back("\n\tmov	esi, " + to_string(posArray));
								code.push_back("\n\tmov	[" + it->first + " + ESI*2], eax");
								cout << "PosCount:" << posCount << "	OP: " << itOp->first << "\n";
								relativo.push_back(posCount+1);
							}else if(!itOp->first.compare("INPUT")){
								/*
									ARQUIVO SAIDA:  INPUT
								*/
								code.push_back("\n\tmov	esi, " + to_string(posArray));
								code.push_back("\n\tpush	eax");
								code.push_back("\n\tmov	eax, 4");
								code.push_back("\n\tmov	ebx, 1");
								code.push_back("\n\tmov	ecx, [" + it->first + " + ESI*2]");
								code.push_back("\n\tmov	edx, 4");
								code.push_back("\n\tpop	eax");
								cout << "PosCount:" << posCount << "	OP: " << itOp->first << "\n";
								relativo.push_back(posCount+1);
							}else if(!itOp->first.compare("OUTPUT")){
								/*
									ARQUIVO SAIDA:  OUTPUT
								*/
								code.push_back("\n\tmov	esi, " + to_string(posArray));
								code.push_back("\n\tpush	eax");
								code.push_back("\n\tmov	eax, 3");
								code.push_back("\n\tmov	ebx, 0");
								code.push_back("\n\tmov	ecx, [" + it->first + " + ESI*2]");
								code.push_back("\n\tmov	edx, 4");
								code.push_back("\n\tpop	eax");
								cout << "PosCount:" << posCount << "	OP: " << itOp->first << "\n";
								relativo.push_back(posCount+1);
							}

							posCount += 2;
						}
					}else{
						_erro = TRUE;
						cout << "ERRO SINTATICO:" << lineCount << ": Operando invalido: :\"" << itOp->first << "\"\n";
					}
	            }

				}
			}
        }else if((it = dirTable.find(token[j])) != dirTable.end()){
			/*Verifica DIREIVAS*/
			if(!it->first.compare("SPACE")){
				/*verifica token */
				str = token[j-1];
				str = str.substr(0, str.length()-1);
				if((it = simbTable.find(str)) != simbTable.end()){
					/*Faz a leitura do espaco a ser reservado para SPACE*/
					str = token[j+1];
					for (string::size_type i = 0; i < str.length(); ++i){
						c=str[i];
						space = space *10 + (c -'0');
					}
					/* salva no vetor 00 space vezes*/
					code.push_back("\tdw\t0");
					space--;
					while(space > 0)
					{
						code.push_back(", 0");
						space--;
						/*
							ARQUIVO SAIDA: SPACE NUM_ESPAÇOS_ALOCADOS
						*/
						//code.push_back("00");
						posCount+= 1;
					}

					/*salva no arquivo de saida o endereco do SPACE e 0's*/
				}else{
					cout << "ERRO SINTATICO:" << lineCount << ": Simbolo inválido: \"" << token[j-1] << "\"\n";
				}
			}else if(!it->first.compare("CONST")){
						str = token[j+1];
						posCount+= 1;
						/*
							ARQUIVO SAIDA:  CONST DEFINITION
						*/
						code.push_back("\tEQU\t" + str);

			}else if(!it->first.compare("SECTION")){
				if(!token[j+1].compare("DATA"))
				{
					code.push_back("\n\nsection .data");

				}else if(!token[j+1].compare("TEXT")){
					code.push_back("\nglobal _start");
					code.push_back("\nsection .text");
					code.push_back("\n_start:");
				}

			}else if(!it->first.compare("EQU")){

			}

		}
		if(token[j] == "\n"){
			lineCount++;
		}
    }

return 0;
}


/*
	INTERRUPCOES DO SISTEMA
*/
/*	INPUT, OUTPUT, C_INPUT, C_OUTPUT, S_INPUT, S_OUTPUT*/
