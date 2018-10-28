#include<bits/stdc++.h>
using namespace std;
typedef enum {
	step_A, step_B, step_C
} base64_encodestep;
typedef struct {
	base64_encodestep step;
	char result;
	int stepcount;
} base64_encodestate;
void base64_init_encodestate(base64_encodestate* state_in);
char base64_encode_value(char value_in);
int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in);
int base64_encode_blockend(char* code_out, base64_encodestate* state_in);
typedef enum {
	step_a, step_b, step_c, step_d
} base64_decodestep;
typedef struct {
	base64_decodestep step;
	char plainchar;
} base64_decodestate;
void base64_init_decodestate(base64_decodestate* state_in);
int base64_decode_value(char value_in);
int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in);
class Base64Encoder {
	public:
		Base64Encoder();
		std::string encode(const std::string &text);
		int encode(const char *inputBuffer, unsigned count, char *encodedBuffer);
		int encodeEnd(char *encodedBuffer);
	private:
		base64_encodestate state;
};
class Base64Decoder {
	public:
		Base64Decoder();
		std::string decode(const std::string &code);
		int decode(const char *inputBuffer, int count, char *decodedBuffer);
	private:
		base64_decodestate state;
};
Base64Decoder::Base64Decoder() { base64_init_decodestate(&state); }
std::string Base64Decoder::decode(const std::string &code) {
	char *buffer = new char[code.length()];
	int numberOfCharacters = base64_decode_block(code.c_str(), code.length(), buffer, &state);
	buffer[numberOfCharacters] = '\0';
	std::string result(buffer);
	delete[] buffer;
	return result;
}
int Base64Decoder::decode(const char *inputBuffer, int count, char *decodedBuffer) {
	return base64_decode_block(inputBuffer, count, decodedBuffer, &state);
}
Base64Encoder::Base64Encoder() { base64_init_encodestate(&state); }
std::string Base64Encoder::encode(const std::string &text) {
	char *buffer = new char[text.length() * 2];
	int numberOfBytesEncoded = base64_encode_block(text.c_str(), text.length(), buffer, &state);
	numberOfBytesEncoded += base64_encode_blockend(buffer + numberOfBytesEncoded, &state);
	buffer[numberOfBytesEncoded] = '\0';
	std::string result(buffer);
	delete[] buffer;
	base64_init_encodestate(&state);
	return result;
}
int Base64Encoder::encode(const char *inputBuffer, unsigned count, char *encodedBuffer) {
	return base64_encode_block(inputBuffer, count, encodedBuffer, &state);
}
int Base64Encoder::encodeEnd(char *encodedBuffer) {
	return base64_encode_blockend(encodedBuffer, &state);
}
int base64_decode_value(char value_in) {
	static const char decoding[] = {62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-2,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
	static const char decoding_size = sizeof(decoding);
	value_in -= 43;
	if (value_in < 0 || value_in >= decoding_size) return -1;
	return decoding[(int)value_in];
}
void base64_init_decodestate(base64_decodestate* state_in) {
	state_in->step = step_a;
	state_in->plainchar = 0;
}
int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in) {
	const char* codechar = code_in;
	char* plainchar = plaintext_out;
	char fragment;
	*plainchar = state_in->plainchar;
	switch (state_in->step) {
		while (1) {
			case step_a:
				do {
					if (codechar == code_in+length_in) {
						state_in->step = step_a;
						state_in->plainchar = *plainchar;
						return plainchar - plaintext_out;
					}
					fragment = (char)base64_decode_value(*codechar++);
				} while (fragment < 0);
				*plainchar    = (fragment & 0x03f) << 2;
			case step_b:
				do {
					if (codechar == code_in+length_in) {
						state_in->step = step_b;
						state_in->plainchar = *plainchar;
						return plainchar - plaintext_out;
					}
					fragment = (char)base64_decode_value(*codechar++);
				} while (fragment < 0);
				*plainchar++ |= (fragment & 0x030) >> 4;
				*plainchar    = (fragment & 0x00f) << 4;
			case step_c:
				do {
					if (codechar == code_in+length_in) {
						state_in->step = step_c;
						state_in->plainchar = *plainchar;
						return plainchar - plaintext_out;
					}
					fragment = (char)base64_decode_value(*codechar++);
				} while (fragment < 0);
				*plainchar++ |= (fragment & 0x03c) >> 2;
				*plainchar    = (fragment & 0x003) << 6;
			case step_d:
				do {
					if (codechar == code_in+length_in) {
						state_in->step = step_d;
						state_in->plainchar = *plainchar;
						return plainchar - plaintext_out;
					}
					fragment = (char)base64_decode_value(*codechar++);
				} while (fragment < 0);
				*plainchar++   |= (fragment & 0x03f);
		}
	}
	return plainchar - plaintext_out;
}
const int CHARS_PER_LINE = 72;
void base64_init_encodestate(base64_encodestate* state_in) {
	state_in->step = step_A;
	state_in->result = 0;
	state_in->stepcount = 0;
}
char base64_encode_value(char value_in) {
	static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	if (value_in > 63) return '=';
	return encoding[(int)value_in];
}
int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in) {
	const char* plainchar = plaintext_in;
	const char* const plaintextend = plaintext_in + length_in;
	char* codechar = code_out;
	char result;
	char fragment;
	result = state_in->result;
	switch (state_in->step) {
		while (1) {
			case step_A:
				if (plainchar == plaintextend) {
					state_in->result = result;
					state_in->step = step_A;
					return codechar - code_out;
				}
				fragment = *plainchar++;
				result = (fragment & 0x0fc) >> 2;
				*codechar++ = base64_encode_value(result);
				result = (fragment & 0x003) << 4;
			case step_B:
				if (plainchar == plaintextend) {
					state_in->result = result;
					state_in->step = step_B;
					return codechar - code_out;
				}
				fragment = *plainchar++;
				result |= (fragment & 0x0f0) >> 4;
				*codechar++ = base64_encode_value(result);
				result = (fragment & 0x00f) << 2;
			case step_C:
				if (plainchar == plaintextend) {
					state_in->result = result;
					state_in->step = step_C;
					return codechar - code_out;
				}
				fragment = *plainchar++;
				result |= (fragment & 0x0c0) >> 6;
				*codechar++ = base64_encode_value(result);
				result  = (fragment & 0x03f) >> 0;
				*codechar++ = base64_encode_value(result);
				++(state_in->stepcount);
				if (state_in->stepcount == CHARS_PER_LINE/4) {
					*codechar++ = '\n';
					state_in->stepcount = 0;
				}
		}
	}
	return codechar - code_out;
}
int base64_encode_blockend(char* code_out, base64_encodestate* state_in) {
	char* codechar = code_out;
	switch (state_in->step) {
		case step_B:
			*codechar++ = base64_encode_value(state_in->result);
			*codechar++ = '=';
			*codechar++ = '=';
			break;
		case step_C:
			*codechar++ = base64_encode_value(state_in->result);
			*codechar++ = '=';
			break;
		case step_A:
			break;
	}
	*codechar++ = '\n';
	return codechar - code_out;
}
void base64EncodeString(const char *textToEncode, char *buffer);
void base64DecodeString(const char *codeToDecode, char *buffer);
void base64EncodeFile(FILE *fileToEncode, FILE *encodedFile);
void base64DecodeFile(FILE *encodedFile, FILE *decodedFile);
string Time();
void print(string msg);
void printusage();
bool cmp(int x,int y);
string version="1.2.0",temp;
char crypt[10][10],
	 headcrypt[10][10]={{7,9,10,11,13,32,33,34,35,36},{37,38,39,40,41,42,43,44,45,46},{47,48,49,50,51,52,53,54,55,56},{57,58,59,60,61,62,63,64,65,66},{67,68,69,70,71,72,73,74,75,76},{77,78,79,80,81,82,83,84,85,86},{87,88,89,90,91,92,93,94,95,96},{97,98,99,100,101,102,103,104,105,106},{107,108,109,110,111,112,113,114,115,116},{117,118,119,120,121,122,123,124,125,126}},
	 uncrypted[100]={7,9,10,11,13,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126};
int main(int argc,char *argv[]) {
	print("MeowCat String Crypter v"+version);
	if (argc<4) {
		printusage();
		return 0;
	}
	temp=argv[1];
	int password=0,tmp=0;
	if (temp=="encrypt") {
		if (argc>=6) {
			temp=argv[5];
			for (register int i=0;i<=temp.length();i++) {
				password+=int(temp[i]);
				password%=198;
			}
		}
	} else if (temp=="decrypt") {
		if (argc>=5) {
			temp=argv[4];
			for (register int i=0;i<=temp.length();i++) {
				password+=int(temp[i]);
				password%=198;
			}
		}
	}
	if (password<=99) {
		sort(&uncrypted[0],&uncrypted[99],cmp);
	}
	for (register int i=1;i<=password;i++) {
		tmp=int(uncrypted[0]);
		for (register int j=0;j<100;j++) {
			uncrypted[i]=uncrypted[i+1];
		}
		uncrypted[99]=char(tmp);
	}
	tmp=0;
	for (register int i=0;i<10;i++) {
		for (register int j=0;j<10;j++) {
			crypt[i][j]=uncrypted[tmp];
			tmp++;
		}
	}
	temp=argv[1];
	if (temp=="encrypt") {
		print("Encrypt detected.");
		if (argc>=5&&argv[4]!="") {
			temp=argv[4];
			print("Head message detected: "+temp+".");
		}
		if (argc>=6&&argv[5]!="") {
			temp=argv[5];
			print("Password detected: "+temp+".");
		}
		FILE *inputFile = fopen(argv[2], "rb");
		string encodedFileNameTemp=argv[2];
		encodedFileNameTemp+=".64";
		const char* encodedFileName=encodedFileNameTemp.data();
	    FILE *encodedFile = fopen(encodedFileName, "wb");
	    print("Encoding file...");
		if (inputFile) {
	        if (encodedFile) {
	            base64EncodeFile(inputFile, encodedFile);
	        } else {
	        	print("Cannot write file.");
	    		print("Encode failed.");
	    		print("Encrypt failed.");
	    		return 0;
	        }
	    } else {
	    	print("Cannot open file.");
	    	print("Encode failed.");
	    	print("Encrypt failed.");
	    	return 0;
	    }
	    fclose(inputFile);
	    fclose(encodedFile);
		int a=0;
		string b="",c="";
		bool d=false;
		FILE *e;
		e=freopen(encodedFileName,"r",stdin);
		freopen(argv[3],"w",stdout);
		if (argc>=5) {
			c=argv[4];
		}
		print("Encrypting...");
		cout<<":MCS:DEC:";
		if (c!="") {
			for (register int i=0;i<=c.length();i++) {
				if (d) {
					d=false;
				}
				for (register int j=0;j<10;j++) {
					for (register int k=0;k<10;k++) {
						if (c[i]==headcrypt[j][k]) {
							cout<<"\n:H:";
							b="";
							for (register int l=1;l<=j+1;l++) {
								b+="|";
							}
							cout<<setw(10)<<b;
							b="";
							for (register int l=1;l<=k+1;l++) {
								b+="|";
							}
							cout<<setw(10)<<b;
							d=true;
							break;
						}
					}
					if (d) {
						break;
					}
				}
			}	
		}
		while (true) {
			a=fgetc(e);
			if (a==EOF) {
				break;
			} else {
				for (register int j=0;j<10;j++) {
					for (register int k=0;k<10;k++) {
						if (char(a)==crypt[j][k]) {
							cout<<"\n:E:";
							b="";
							for (register int l=1;l<=j+1;l++) {
								b+="|";
							}
							cout<<setw(10)<<b;
							b="";
							for (register int l=1;l<=k+1;l++) {
								b+="|";
							}
							cout<<setw(10)<<b;
							d=true;
							break;
						}
					}
					if (d) {
						break;
					}
				}
				if (d) {
					d=false;
				} else {
					cout<<"\n:D:"<<a;
				}
			}
		}
		print("Removing temporary file...");
	    ofstream outfile;
		outfile.open(encodedFileName,ios::trunc);
		remove(encodedFileName);
		print("Encrypt completed.");
		fclose(stdin);
		fclose(stdout);
		return 0;
	} else if (temp=="decrypt") {
		print("Decrypt detected.");
		if (argc>=5) {
			temp=argv[4];
			if (temp!="") {
				print("Password detected: "+temp+".");
			}
		}
		bool f=true,h=false;
		ifstream g(argv[2]);
		string a="",b="";
		int c=0,d=0;
		string decodedFileNameTemp=argv[2];
		decodedFileNameTemp+=".64";
		const char* decodedFileName=decodedFileNameTemp.data();
		freopen(decodedFileName,"w",stdout);
		if (!g) {
			print("Cannot open file.");
			print("Decrypt Failed.");
			fclose(stdout);
			return 0;
		}
		while (!g.eof()) {
			getline(g,a);
			if (!f&&!h) {
				cerr<<"["+Time()+"] Head message: ";
				h=true;
			}
			if (f) {
				if (a!=":MCS:DEC:") {
					print("File is not a stanard MeowCat Studio Decrypt file.");
					print("Decrypt Failed.");
					fclose(stdout);
					return 0;
				} else {
					f=false;
					print("Decrypting...");
				}
			} else {
				b="";
				for (register int i=0;i<3;i++) {
					b+=a[i];
				}
				if (b==":E:") {
					c=0;
					for (register int i=3;i<13;i++) {
						if (a[i]=='|') {
							c++;
						}
					}
					d=0;
					for (register int i=13;i<23;i++) {
						if (a[i]=='|') {
							d++;
						}
					}
					cout<<crypt[c-1][d-1];
				} else if (b==":D:") {
					cout<<char(a[3]);
				} else if (b==":H:") {
					c=0;
					for (register int i=3;i<13;i++) {
						if (a[i]=='|') {
							c++;
						}
					}
					d=0;
					for (register int i=13;i<23;i++) {
						if (a[i]=='|') {
							d++;
						}
					}
					cerr<<headcrypt[c-1][d-1];
				}
			}
		}
		cerr<<endl;
		FILE *inputFile = fopen(decodedFileName, "rb");
	    FILE *encodedFile = fopen(argv[3], "wb");
	    print("Decoding...");
		if (inputFile) {
	        if (encodedFile) {
	            base64DecodeFile(inputFile, encodedFile);
	        } else {
	        	print("Cannot write file.");
	        	print("Decode failed.");
	    		print("Decrypt failed.");
	    		return 0;
	        }
	    } else {
	    	print("Cannot open file.");
	        print("Decode failed.");
	    	print("Decrypt failed.");
	    	return 0;
	    }
	    print("Removing temporary file...");
	    ofstream outfile;
		outfile.open(decodedFileName,ios::trunc);
		remove(decodedFileName);
		print("Decrypt completed.");
	    fclose(inputFile);
	    fclose(encodedFile);
		fclose(stdout);
		return 0;
	} else {
		printusage();
		return 0;
	}
	return 0;
}
string Time() {
	time_t t=time(NULL);
	char c[64]={0};
	strftime(c,sizeof(c)-1,"%H:%M:%S",localtime(&t));
	return c;
}
void print(string msg) {
	cerr<<"["+Time()+"] "+msg+"\n";
}
void printusage() {
	print("Usage:");
	print("Encrypt: StringCrypter encrypt <file> <out> [head] [password]");
	print("Decrypt: StringCrypter decrypt <file> <out> [password]");
	print("This program is NOT REAL encrypt,just for fun.");
}
bool cmp(int x,int y) {
	return x>y;
}
void base64EncodeString(const char *textToEncode, char *buffer) {
    base64_encodestate state;
    base64_init_encodestate(&state);
    int numberOfBytesEncoded = base64_encode_block(textToEncode, strlen(textToEncode), buffer, &state);
    numberOfBytesEncoded += base64_encode_blockend(buffer + numberOfBytesEncoded, &state);
    buffer[numberOfBytesEncoded] = '\0';
}
void base64DecodeString(const char *codeToDecode, char *buffer) {
    base64_decodestate state;
    base64_init_decodestate(&state);
    int numberOfCharactersDecoded = base64_decode_block(codeToDecode, strlen(codeToDecode), buffer, &state);
    buffer[numberOfCharactersDecoded] = '\0';
}
void base64EncodeFile(FILE *fileToEncode, FILE *encodedFile) {
    char readBuffer[1024];
    char encodedBuffer[2048];
    int numberOfBytesEncoded;
    base64_encodestate state;
    base64_init_encodestate(&state);
    while (!feof(fileToEncode)) {
        size_t numberOfObjects = fread(readBuffer, sizeof(char), 1024, fileToEncode);
        numberOfBytesEncoded = base64_encode_block(readBuffer, numberOfObjects, encodedBuffer, &state);
        fwrite(encodedBuffer, sizeof(char), numberOfBytesEncoded, encodedFile);
    }
    numberOfBytesEncoded = base64_encode_blockend(encodedBuffer, &state);
    fwrite(encodedBuffer, sizeof(char), numberOfBytesEncoded, encodedFile);
}
void base64DecodeFile(FILE *encodedFile, FILE *decodedFile) {
    char inputBuffer[1024];
    char decodedBuffer[2048];
    int numberOfBytesDecoded;
    base64_decodestate state;
    base64_init_decodestate(&state);
    while (!feof(encodedFile)) {
        size_t numberOfCharacters = fread(inputBuffer, sizeof(char), 1024, encodedFile);
        numberOfBytesDecoded = base64_decode_block(inputBuffer, numberOfCharacters, decodedBuffer, &state);
        fwrite(decodedBuffer, sizeof(char), numberOfBytesDecoded, decodedFile);
    }
}
