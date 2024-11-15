/*
 Declaration of Authorship
 Name : Prabhat Kumar Malviya
 Roll No. 2101CS58
*/
#include <bits/stdc++.h>

using namespace std;

// Defining structure for literals
struct literal
{
   int literal;
   int address;
};

// defining structure for symbols
struct symbol
{
   string name;
   int address;
   bool set;
   int set_value;
};

// vector to store symbols
vector<symbol> symbolt;

// vector to store literals
vector<literal> literalt;

// function to remove blank spaces
static inline string& rm_space(string& s)
{
   s.erase(find_if(s.rbegin(), s.rend(),
      not1(ptr_fun<int, int>(isspace)))
      .base(),
      s.end());
   s.erase(s.begin(), find_if(s.begin(), s.end(),
      not1(ptr_fun<int, int>(isspace))));
   return s;
}

// function to match elements from symbol table
bool ispresent(string name)
{
   auto iter = find_if(symbolt.begin(), symbolt.end(),
      [&](const symbol& ts)
      { return ts.name == name; });
   return iter != symbolt.end();
}

// label validity checking function
int label_verify(string label)
{
   if (!((label[0] >= 'a' && label[0] <= 'z') || (label[0] >= 'A' && label[0] <= 'Z')))
      return false;

   for (int i = 0; i < label.length(); i++)
   {
      if (!(isdigit(label[i]) || (label[0] >= 'a' && label[0] <= 'z') || (label[0] >= 'A' && label[0] <= 'Z')))
      {
         return false;
      }
   }
   return true;
}

// function to check if the given string is a number pr not
bool num_check(const std::string& s)
{
   int i = 0;
   string tpp;
   tpp = s;
   if (s.front() == '-' or s.front() == '+')
   {
      tpp = s.substr(1, s.length());
   }
   else if (s.find("0x") == 0)
   {
      tpp = s.substr(2), 16;
   }
   std::string::const_iterator it = tpp.begin();
   while (it != tpp.end() && std::isdigit(*it))
      ++it;
   return !tpp.empty() && it == tpp.end();
}

// int to hex convertion function
string int_to_hex(int i)
{
   stringstream stream;
   stream << setfill('0') << setw(8) << hex << i;
   return stream.str();
}

// function to convert the string to decimal
int tonum(string s)
{
   s = rm_space(s);
   if (s.find("-") == 0)
   {
      return -stoul(s.substr(1, s.length()), nullptr, 10);
   }
   else if (s.find("+") == 0)
   {
      return stoul(s.substr(1, s.length()), nullptr, 10);
   }
   else if (s.find("0x") == 0)
   {
      return stoul(s, nullptr, 16);
   }
   else if (s.find("0") == 0)
   {
      return stoul(s, nullptr, 8);
   }
   else
   {
      return stoul(s, nullptr, 10);
   }
}

// MOT table
map<string, string> machineop;
void Machine_init()
{
   machineop["ldc"] = string("00");
   machineop["adc"] = string("01");
   machineop["ldl"] = string("02");
   machineop["stl"] = string("03");
   machineop["ldnl"] = string("04");
   machineop["stnl"] = string("05");
   machineop["add"] = string("06");
   machineop["sub"] = string("07");
   machineop["shl"] = string("08");
   machineop["shr"] = string("09");
   machineop["adj"] = string("0a");
   machineop["a2sp"] = string("0b");
   machineop["sp2a"] = string("0c");
   machineop["call"] = string("0d");
   machineop["return"] = string("0e");
   machineop["brz"] = string("0f");
   machineop["brlz"] = string("10");
   machineop["br"] = string("11");
   machineop["HALT"] = string("12");
   machineop["data"] = string("13");
   machineop["SET"] = string("14");
}

// Reading instructions and add into literal and symbol table
string add_inst(string instr, int* loc_ptr, int line)
{
   int loc = *loc_ptr;
   string errors = "";

   // Identify label and variables
   if (instr.find(':') != string::npos)
   {
      int colon = instr.find(":", 0);

      if (ispresent(instr.substr(0, colon - 1)))
      {
         errors += "Error at line " + to_string(line) + ": Duplicate label\n";
      }

      if (label_verify(instr.substr(0, colon - 1)) == false)
      {
         errors += "Warning at line " + to_string(line) + ": Incorrect label format\n";
      }

      // Instruction could be present after the colon
      if (colon == instr.length() - 1)
      {
         *loc_ptr = *loc_ptr - 1;
         symbolt.push_back({ instr.substr(0, colon), loc, 0, -1 });
      }
      else
      {
         string subs = instr.substr(colon + 1, instr.length());
         subs = rm_space(subs);
         add_inst(subs, &loc, line);
         int space = subs.find(" ", 0);
         string sub_op = subs.substr(0, space);
         sub_op = rm_space(sub_op);
         string sub_val = subs.substr(space + 1, subs.length());
         sub_val = rm_space(sub_val);

         // Dealing with set instructions
         if (sub_op != "SET")
         {
            symbolt.push_back({ instr.substr(0, colon), loc, 0, -1 });
         }
         else
         {
            symbolt.push_back({ instr.substr(0, colon), loc, 1, stoi(sub_val) });
         }
      }
   }

   // Identify literals and constants
   else
   {
      // Considering that second part of instruction is operand
      int space = instr.find(" ", 0);
      string subs = instr.substr(space + 1, instr.length());
      subs = rm_space(subs);

      // Checking for numeral
      if (num_check(subs))
      {
         literalt.push_back({ tonum(subs), -1 });
      }
   }
   return (errors);
}

void _pass1(string file, ofstream& logfile)
{
   // tpp str variable for get line
   string line;
   int loc = 0, line_count = 1;

   // Reading the input file
   ifstream inp_file(file);

   // Read individual lines
   while (getline(inp_file, line))
   {
      // Modifying the line, removing spaces
      string instr;
      instr = line.substr(0, line.find(";", 0));
      instr = rm_space(instr);

      // excluding empty lines
      if (instr == "")
      {
         line_count++;
         continue;
      }

      logfile << add_inst(instr, &loc, line_count++);
      loc++;
   }

   // LTORG loop for literals
   for (int i = 0; i < literalt.size(); i++)
   {
      if (literalt[i].address == -1)
      {
         literalt[i].address = loc++;
      }
   }
   inp_file.close();
}

tuple<string, string, string> inst_to_code(string instr, int* loc_ptr, int line)
{
   // Program Counter
   int loc = *loc_ptr;

   // Hex Code of Program Counter
   string encd = int_to_hex(loc) + " ";

   // Warnings and errors
   string encd_ = "";
   string errors = "";
   string mcode = "";

   // Identify label and variables
   if (instr.find(':') != string::npos)
   {
      int colon = instr.find(":", 0);

      // Instruction could be present after the colon
      if (colon != instr.length() - 1)
      {
         string subs = instr.substr(colon + 1, instr.length());
         subs = rm_space(subs);
         tie(encd_, errors, mcode) = inst_to_code(subs, &loc, line);
         string tpp = encd_;
         tpp = tpp.substr(9, 9);
         encd += tpp;
      }
      // If no instruction after colon, PC shouldn't change
      else
      {
         encd += "         ";
         *loc_ptr = *loc_ptr - 1;
      }
      encd += instr + "\n";
   }

   // Identify literals and constants
   else
   {
      // Considering that second part of instruction is operand
      int space = instr.find(" ", 0);

      // tpp variable for operand encoding
      string hexstr;

      // String containing operation
      string sub_op = instr.substr(0, space);

      // String containing operand
      string sub_operand = instr.substr(space + 1, instr.length());

      sub_operand = rm_space(sub_operand);
      sub_op = rm_space(sub_op);

      // Checking for invalid mnemonics
      if (machineop[sub_op] == "")
      {
         errors += "Error at line " + to_string(line) + ": Mnemonic not defined\n";
      }

      // Checking for No operand instructions
      else if (sub_op == "add" || sub_op == "sub" || sub_op == "shl" || sub_op == "shr" || sub_op == "a2sp" || sub_op == "sp2a" || sub_op == "return" || sub_op == "HALT")
      {
         encd += "000000" + machineop[sub_op] + " ";
         mcode += "000000" + machineop[sub_op] + " ";
         if (sub_op.length() != instr.length())
         {
            errors += "Error at line" + to_string(line) + ": Unexpected operand\n";
         }
      }
      // Checking for numeral operand to encode directly
      else if (num_check(sub_operand))
      {
         hexstr = int_to_hex(tonum(sub_operand));
         // if(sub_op == "SET" || sub_op == "data")
         //     encoding += hexstr + " ";
         // else
         //     encoding += hexstr.substr(hexstr.length() - 6, hexstr.length()) + machineop[sub_op] + " ";
         encd += hexstr.substr(hexstr.length() - 6, hexstr.length()) + machineop[sub_op] + " ";
         mcode += hexstr.substr(hexstr.length() - 6, hexstr.length()) + machineop[sub_op] + " ";
      }
      // Checking for variable operand to encode address
      else
      {
         int defined = 0;
         for (int i = 0; i < symbolt.size(); i++)
         {
            if (symbolt[i].name.compare(sub_operand) == 0)
            {
               defined = 1;
               // SET variables considered numeral
               if (symbolt[i].set)
               {
                  hexstr = int_to_hex(symbolt[i].set_value);
               }
               // Operands which need offset from PC
               else if (sub_op == "call" || sub_op == "brz" || sub_op == "brlz" || sub_op == "br")
               {
                  hexstr = int_to_hex(symbolt[i].address - loc - 1);
               }
               // Normal case, encode address
               else
               {
                  hexstr = int_to_hex(symbolt[i].address);
               }
               encd += hexstr.substr(hexstr.length() - 6, hexstr.length()) + machineop[sub_op] + " ";
               mcode += hexstr.substr(hexstr.length() - 6, hexstr.length()) + machineop[sub_op] + " ";
               break;
            }
         }
         if (sub_op.length() == instr.length())
         {
            errors += "Error at line " + to_string(line) + ": Missing operand" + "\n";
         }
         else if (!defined)
         {
            errors += "Error at line " + to_string(line) + ": Unknown Symbol" + "\n";
         }
      }
      encd += instr + "\n";
   }
   return (make_tuple(encd, errors, mcode));
}

void _pass2(string file, ofstream& outfile, ofstream& logfile, ofstream& objfile)
{
   // tpp str variable for get line
   string line;

   // Program,line Counter
   int loc = 0, line_count = 1;

   // Reading the input file
   ifstream inp_file(file);

   // Read individual lines
   while (getline(inp_file, line))
   {
      // Pre-process the line, rm_space extra spaces
      string instr;
      string encd;
      string errors;
      string mcode;

      instr = line.substr(0, line.find(";", 0));
      instr = rm_space(instr);

      // Skip empty lines
      if (instr == "")
      {
         line_count++;
         continue;
      }

      tie(encd, errors, mcode) = inst_to_code(instr, &loc, line_count++);
      outfile << encd;
      // objfile << mcode;
      // uint32_t tpp = streamtohex("0x" + mcode);
      if (mcode != "")
      {
         uint32_t tpp = stoul("0x" + rm_space(mcode), nullptr, 16);
         objfile.write((char*)&tpp, sizeof(tpp));
      }
      logfile << errors;
      loc++;
   }
}

int main(int argc, char* argv[])
{
   // Initalize Machine operation table
   Machine_init();

   // Argument for input file
   string in_file = argv[1];

   // Argument for output file
   string opfile = in_file.substr(0, in_file.find(".", 0)) + ".L";

   // Argument for log file
   string log_file = in_file.substr(0, in_file.find(".", 0)) + ".log";

   // Argument for object file
   string obj_file = in_file.substr(0, in_file.find(".", 0)) + ".o";

   // Defining output listing, log file
   ofstream outfile(opfile);
   ofstream logfile(log_file);
   ofstream objfile(obj_file, ios::out | ios::binary);

   // Pass-1 of assembly, analysis phase
   _pass1(in_file, logfile);

   // Pass-2 of assembly, synthesis phase
   _pass2(in_file, outfile, logfile, objfile);

   // Close files
   outfile.close();
   logfile.close();
   objfile.close();
}