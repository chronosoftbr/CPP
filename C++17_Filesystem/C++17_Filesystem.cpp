//
// C++17_Filesystem_Demo.cpp
//

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>
#include <time.h>
#include <windows.h>
#include <tuple>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include "sha2.h"

using namespace std;
using namespace std::filesystem;
using namespace std::chrono;

// Data
class filesHash
{
public:
  string hsh;
  path pathway;
  path pathway1;

  filesHash()
  {
  }
  filesHash(path camin1)
  {
    pathway1 = camin1;
  }
  filesHash(string hs, path camin, path camin1)// : hsh(hs), pathway(camin)  // Construtor
  {
    hsh = hs;
    pathway = camin;
    pathway1 = camin1;
  }
};

struct fileHashCompare  // Compare 2 Hash objects using his contents
{
  bool operator()(const filesHash& hsh1, const filesHash& hsh2)
  {
    if (hsh1.hsh == hsh2.hsh)
      return &hsh1 < &hsh2;
    return hsh1.hsh < hsh2.hsh;
  }
};

bool search_d = false;
bool search_a = false;
bool create_f = false;
int errors = 0;
int isfile = 0;
typedef string hsh;
typedef tuple<path, file_status, uintmax_t, hsh> item;
list<item> items;
list< filesHash > s256;
list< filesHash > s256_p;

// Functions
static tuple<path, file_status, uintmax_t, hsh> file_info(const directory_entry& entry);
void printPerms(perms perm);

int main(int argc, char* argv[])
{
  using namespace std;

  filesHash fhash;
  path dirPath, inpdirPath;
  path ResultFilePath;
  path InputFilePath;
  string dir, inpdir, fil, par;
  clock_t t;
  int n = 0;
  int m = 0;
  int cnt = 0;

  SetConsoleOutputCP(1252);
  SetConsoleCP(1252);

  // for test only...
/*
    while (argc--)
    {
      cout << "\nargc_" << argc << " = " << argv[argc];
    }
*/

  if (argc > 1)
  {
    dirPath = (argv[1]);
    if (argc > 2)
    {
      ResultFilePath = (argv[2]);
    }
    else
    {
      cout << "\nError - too few parameters...";
      cout << "\nDuplicator usage: duplicator + folder + result file + -p(print option)\n";
      return -2;
    }

    if (argc == 4)
    {
      par = argv[3];
      if (operator==(par, "-p"))
      {
        create_f = true;
        cout << "\nPrinting...";
      }
    }
  }
  else
  {
    cout << "\nEnter folder root name (i.e. c:\\mydir\\): ";
    getline(cin, dir);
    dirPath = dir;

    cout << "\nEnter TXT result file name (i.e. c:\\myresulty.txt): ";
    getline(cin, fil);
    ResultFilePath = fil;

    cout << "\nEnter one Parameter:";
    cout << "\n\t-d = search for duplicated files";
    cout << "\n\t-p = create a filename-hash file";
    cout << "\n\t-a = search for altered files\n";
    getline(cin, par);
    if (operator==(par, "-d"))
      search_d = true;
    else
    {
      if (operator==(par, "-p"))
        create_f = true;
      else
      {
        if (operator==(par, "-a"))
          search_a = true;
        else
        {
          cout << "\n\tNo valid parameter!\n";
          return -7;
        }
      }
    }
  }

  if (!exists(dirPath))
  {
    cout << "Error - Path " << dirPath << " does not exist.\n";
    return -1;
  }

  ofstream result_file(ResultFilePath, ios::out);  // implicit conversion
  if (!result_file)
  {
    cout << "Error - File " << ResultFilePath << " was not created.\n";
    return -5;
  }

  result_file.clear();

  if (search_d)
  {
    cout << "\n\nSearching for duplicated files in folder " << dirPath << " and subfolders.\n\n";
  }

  if (create_f)
  {
    cout << "\nCreating a filename-hash file... ";
  }

  if (search_a)
  {
    cout << "\nEnter TXT hash file to be searched (i.e. c:\\myHash.txt): ";
    getline(cin, inpdir);
    inpdirPath = inpdir;
  }

  // start time
  t = clock();
  const time_t td = time(0);
  tm tt;
  localtime_s(&tt, &td);
  result_file << "\n\t\t" << tt.tm_mday << "/" << tt.tm_mon + 1 << "/" << tt.tm_year + 1900
    << "\t" << tt.tm_hour << ":" << tt.tm_min << "\n\n";

  // Mount all folders and subfolders
  try
  {
    transform(recursive_directory_iterator{ dirPath }, {}, back_inserter(items), file_info);
  }
  catch (const filesystem_error& e)
  {
    cout << "Catch... " << e.what() << '\n';
  }

  // Put some Items into S256 hash lists
  for (const auto& [path, status, size, myHash2] : items)
  {
    if (is_directory(status))
    {
      n++;
      //    result_file << path << "\n";
      //    wcout << "\nDiretorio " << path.relative_path() << endl;
    }
    else
    {
      m++;
      s256.push_back(filesHash(myHash2, path, "  "));
      s256_p.push_back(filesHash(myHash2, path, "  "));
      //result_file << "\t" << path << "\n";
    }
  }

  // -p option create a filename-hash file
  if (create_f)
  {
    for (filesHash& fileshash : s256_p)
    {
      result_file << fileshash.pathway << " = " << fileshash.hsh << "\n";
      cnt++;
    }

    wcout << "\n\t\t" << cnt << " Files in " << t / 1000.0 << " seconds. " << endl;

    result_file.close();
    return 0;
  }

  s256.sort(fileHashCompare());
  s256_p.sort(fileHashCompare());

  // -d option search for duplicated files
  if (search_d)
  {
    string file1s = " ";

    result_file << "\tDuplicated files in folder " << dirPath << " and subfolders.\n";
    for (filesHash& fileshash : s256)
    {
      if (file1s.compare(fileshash.hsh) == 0)
      {
        //          cout << "\nThe file " << file1p << "\n\t" << file1s << " and";
        //          cout << "\nThe file " << fileshash.pathway << "\n\t" << fileshash.hsh << " are duplicated" << endl;
        result_file << fhash.pathway1 << "\n" << fileshash.pathway << "\n\n";
        cnt++;
      }
      file1s = fileshash.hsh;
      fhash.pathway1 = fileshash.pathway;
    }
    result_file.close();
    t = clock() - t;
    wcout << "\n\t\t" << m << " Files in " << n << " Folders. " << cnt << " Duplicated files in " << t / 1000.0 << " seconds. " << endl;
    wcout << "\t\t" << errors << " Failed to open files and " << isfile << " not identified files type." << endl;
    return(0);
  }

  // -a option search for altered files
  if (search_a)
  {
    char altered[100];

    ifstream input_file(inpdirPath, ios::in);
    if (!input_file)
    {
      cout << "Error - File " << InputFilePath << " does not exists.\n";
      return -11;
    }

    while (!input_file.eof())
    {
      input_file.getline(altered, sizeof(altered));
      for (filesHash& fileshash : s256)
      {
        /*
                if (fname.compare(fileshash.pathway) == 0)
                {
                  //          cout << "\nThe file " << file1p << "\n\t" << file1s << " and";
                  //          cout << "\nThe file " << fileshash.pathway << "\n\t" << fileshash.hsh << " are duplicated" << endl;
                  result_file << "Not Altered " << "\n" << fileshash.pathway << "\n\n";
                }
        */
        //if (file1s.compare(fileshash.hsh) == 0)
        if (fileshash.hsh.compare(altered) == 0)
        {
          //          cout << "\nThe file " << file1p << "\n\t" << file1s << " and";
          //          cout << "\nThe file " << fileshash.pathway << "\n\t" << fileshash.hsh << " are duplicated" << endl;
          result_file << "Found " << "\n" << fileshash.pathway << "\n\n";
        }
      }
    }
    result_file.close();
    input_file.close();
  }
}

static tuple<path, file_status, uintmax_t, hsh> file_info(const directory_entry& entry)
{
  SHA256 sha256;
  int numbytes = 0;
  string myHash2;
  const auto fs(status(entry));
  string fname;
  size_t nbytes;
  char ffname[100];

  if (is_block_file(fs))
  {
    return { entry.path(), fs, 0, "C" };
  }
  if (is_character_file(fs))
  {
    return { entry.path(), fs, 0, "Error 2" };
  }
  if (is_directory(fs))
  {
    return { entry.path(), fs, 0, "Error 3" };
  }
  //if (is_empty(fs)) does not exist in Windows
  if (is_fifo(fs))
  {
    return { entry.path(), fs, 0, "Error 4" };
  }
  if (is_other(fs))
  {
    return { entry.path(), fs, 0, "Error 5" };
  }
  if (is_socket(fs))
  {
    return { entry.path(), fs, 0, "Error 6" };
  }
  if (is_symlink(fs))
  {
    return { entry.path(), fs, 0, "Error 7" };
  }
  if (is_regular_file(fs))
  {
    ifstream myFile(entry.path(), ifstream::binary);
    uintmax_t myfileSize = file_size(entry.path());
    //myFile.open()
    if (myFile.is_open() == true)
    {        // myHash2 = { "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" };
      if (myfileSize == 0)
      {
        //  if (create)
        //    std::cout << "Hash of " << numbytes << " bytes of " << entry.path() << " is e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" << endl;
        myFile.close();
        return { entry.path(), fs, file_size(entry.path()), "zero bytes file" };
      }

      // Insert filename
      fname = entry.path().filename().string();
      nbytes = fname.length();
      //vector<char> file_data(myfileSize + nbytes);
      vector<char> file_data(myfileSize);
      copy(fname.begin(), fname.end(), ffname);
      ffname[nbytes + 1] = '\0';
      /*
              // put filename in the begin of file
              // copy(&file_data[0], &file_data[nbytes], ffname); This function does not work!
              copy_n(ffname, nbytes, &file_data[0]);
              myFile.read(&file_data[nbytes], myfileSize);
      */
      /*
              // put filename in the end of file
              myFile.read(&file_data[0], myfileSize);
              copy_n(ffname, nbytes, &file_data[myfileSize]);
      */
      myFile.read(&file_data[0], myfileSize);
      size_t file_sz = myfileSize;
      //     uintmax_t file_sz = myfileSize + nbytes;
      while (file_sz)
      {
        if (file_sz <= 80)
        {
          sha256.add(&file_data[numbytes], file_sz);
          //  numbytes += (int)file_sz;
          file_sz = 0;
        }
        else
        {
          file_sz -= 80;
          sha256.add(&file_data[numbytes], 80);
          numbytes += 80;
        }
      }
      myHash2 = sha256.getHash();
      // if (create)
      //   std::cout << "Hash of " << numbytes << " bytes of " << entry.path() << " is " << myHash2 << endl;
    }
    else
    {
      errors++;
      cerr << errors << " Failed to open file - Permitions:\n " << entry.path() << endl;
      //  printPerms(status(entry.path()).permissions());
    }
    myFile.close();
  }
  else
  {
    isfile++;
  }
  return { entry.path(), fs, file_size(entry.path()), myHash2 };
}

void printPerms(perms perm)
{
  int permi = (int)perm;
  std::cout << hex << permi << endl;

  std::cout << "\nowner " << ((perm & perms::owner_read) != perms::none ? "r" : "-")
    << ((perm & perms::owner_write) != perms::none ? "w" : "-")
    << ((perm & perms::owner_exec) != perms::none ? "x" : "-")
    << "\ngroup " << ((perm & perms::group_read) != perms::none ? "r" : "-")
    << ((perm & perms::group_write) != perms::none ? "w" : "-")
    << ((perm & perms::group_exec) != perms::none ? "x" : "-")
    << "\nothers " << ((perm & perms::others_read) != perms::none ? "r" : "-")
    << ((perm & perms::others_write) != perms::none ? "w" : "-")
    << ((perm & perms::others_exec) != perms::none ? "x" : "-")
    << endl;
}
