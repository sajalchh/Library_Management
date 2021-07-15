#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <functional>
#include <dirent.h>
#include <sys/types.h>
#include <string>
#include <cstdlib>
#include <bits/stdc++.h>
#include <sstream>

using namespace std;

class Book{
    public:
    string title,author,file_name;
    int type,ID;
    int start;
    Book(string fileName):file_name(fileName){}
    void print(){
        cout<<"ID: "<<ID<<endl<<title<<endl<<author<<endl<<"File name: "<<file_name<<endl<<endl;
    }

};

class Novel : public Book{
    public:
    vector <long long int> para;
    vector <long long int> chapters;
    Novel(string a):Book(a){}
};
class Play : public Book{
    public:
    vector<string> characters;
    Play(string a):Book(a){}
};

void printInstruct(){
    cout<<"Enter the corresponding number to do any of the following:\n"<<
    " 1: List all books along with their filename, title,and author name.\n"<<
    " 2: Search for a book by its title or author name, and show the matching books as a list.\n"<<
    " 3: Display a book from any of the above two lists.\n"<<
    " 4: For the analytics task(case 3 of the assignment).\n"<<
    "-1: To exit.\n";
}

bool findString(const string & strA, const string & strB)
{
  auto it = search(
    strA.begin(), strA.end(),
    strB.begin(),   strB.end(),
    [](char ch1, char ch2) { return toupper(ch1) == toupper(ch2); }
  );
  return (it != strA.end() );
}

bool comp(pair<int,string> a,pair<int,string> b){return (a.first>b.first);}

string upCase(string a){
    int n=a.length();
    for(int i=0;i<n;i++){
        if(a[i]>='a'&&a[i]<='z'){
            a[i]=a[i]+'A'-'a';
        }
    }
    return a;
}

int main(){
    string path="../assign_cpp_2/Directory";
    cout<<"Welcome to the Library Management Sytem.\n";
    cout<<"The Default path for the books is:   '"<<path<<"'\nDo you want to change it?\nEnter Y if Yes or N if No :\n";
    char ty;
    cin>>ty;
    if(ty=='Y'||ty=='y'){
        cout<<"Enter the new path: ";
        cin>>path;
    }
    const char* finalPath=&path[0];
    vector <string> dirFiles;
    map <string,int> fileNo;
    DIR *dir;
    struct dirent *ent;
    int index=-1,i=0,j=0,n=0;

    string temp;
    if ((dir = opendir (finalPath)) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
        temp=ent->d_name;
        if(temp.find(".txt")!=string::npos){
            i++;
            dirFiles.push_back(temp);
            fileNo[temp]=i;
            if(dirFiles[i-1].compare("index.txt")==0){
                dirFiles.pop_back();
                i--;
            }
        }
    }
    closedir (dir);
    } else {
    perror ("");
    return EXIT_FAILURE;
    }
    fstream ind;
    string index_path=path+"/index.txt";
    ind.open(index_path,ios::trunc|ios::out|ios::in);
    int totalFiles=dirFiles.size();
    for(i=0;i<totalFiles;i++){
        ind<<dirFiles[i]<<endl;
    }
    // ind.seekg(0, ios::beg);
    ind.close();
    map <string, int> typemap; //1 for Play and 2 for Novel
    map <string, int> id;
    vector <Play> plays;
    vector <Novel> novels;
    fstream read;
    int k;
    cout<<"Please Enter the corresponding letter for the book type of the following files:\nP:Play\nN:Novel\n";
    for(i=0;i<totalFiles;i++){
        cout<<"For '"<<dirFiles[i]<<"' :\n";
        cin>>ty;
        read.open(path+"/"+dirFiles[i],ios::in);
        temp="";
        if(ty=='P'||ty=='p'){
            typemap[dirFiles[i]]=1;
            id[dirFiles[i]]=plays.size();
            Play tmpp(dirFiles[i]);
            plays.push_back(tmpp);
             while(temp.find("Title:")==string::npos){
                getline(read,temp);
            }
            plays[id[dirFiles[i]]].title=temp;
            while(temp.find("Author:")==string::npos){
                getline(read,temp);
            }
            plays[id[dirFiles[i]]].author=temp;
            while(temp.find("*** START OF THIS PROJECT")==string::npos){
                getline(read,temp);
            }
            // cout<<read.tellg();
            // read.seekg(read.tellg()-20,ios::beg);
            // getline(read,temp);
            // cout<<temp<<endl;
            plays[id[dirFiles[i]]].start=read.tellg();
            plays[id[dirFiles[i]]].ID=plays.size()-1;

        }
        else if(ty=='N'||ty=='n'){
            typemap[dirFiles[i]]=2;
            id[dirFiles[i]]=novels.size();
            Novel tmpn(dirFiles[i]);
            novels.push_back(tmpn);
            while(temp.find("Title:")==string::npos){
                getline(read,temp);
            }
            novels[id[dirFiles[i]]].title=temp;
            while(temp.find("Author:")==string::npos){
                getline(read,temp);
            }
            novels[id[dirFiles[i]]].author=temp;
            while(temp.find("*** START OF THIS PROJECT")==string::npos){
                getline(read,temp);
            }
            novels[id[dirFiles[i]]].start=read.tellg();
            novels[id[dirFiles[i]]].ID=novels.size()-1;
        }
        else{
            cout<<"Invalid book type input. Try again.\n";
            i--;
        }
        read.close();
    }

    int totalPlays=plays.size(),totalNovels=novels.size();
    bool check;
    string word;
    int choice,choice2;
    do{
        printInstruct();
        cin>>choice;
        switch(choice){
            case 1: 
                if(plays.size()!=0)
                cout<<"All Plays are:\n\n";
                for(i=0;i<totalPlays;i++){
                    plays[i].print();
                }
                if(novels.size()!=0)
                cout<<"All Novels are:\n\n";
                for(i=0;i<totalNovels;i++){
                    novels[i].print();
                }
                break;
            case 2: 
                cout<<"Search: ";
                cin>>temp;
                cout<<"\nSearch results:\n";
                check=true;
                for(i=0;i<totalPlays;i++){
                    if(findString(plays[i].author,temp)||findString(plays[i].title,temp)){
                        plays[i].print();
                        check=false;
                    }
                }
                for(i=0;i<totalNovels;i++){
                    if(findString(novels[i].author,temp)||findString(novels[i].title,temp)){
                        novels[i].print();
                        check=false;
                    }
                }
                if(check){
                    cout<<"No results found\n";
                }
                break;
            case 3:
                cout<<"Do you want to read a novel or a play.\nEnter N for novel and P for play\n :";
                cin>>ty;
                cout<<"Enter the ID of the book you want to read\n :";
                cin>>index;
                if(ty=='P'||ty=='p'){
                    if(index>=totalPlays){
                        cout<<"Invalid Input.Check the Id with search or display all once again.\n";
                        break;
                    }
                    read.open(path+"/"+plays[index].file_name,ios::in);
                    cout<<plays[index].title;
                    read.seekg(plays[index].start,ios::beg);
                    do{
                        i=50;
                        while(i--){
                        getline(read,temp);
                        cout<<temp<<endl;
                        }
                        cout<<"Enter 1: for next page\n-1: to close the book\n :";
                        cin>>choice2;
                    }
                    while(choice2==1);
                    read.close();
                }
                else if(ty=='N'||ty=='n'){
                    if(index>=totalNovels){
                        cout<<"Invalid Input.Check the Id with search or display all once again.\n";
                        break;
                    }
                    read.open(path+"/"+novels[index].file_name,ios::in);
                    read.seekg(novels[index].start,ios::beg);
                    cout<<novels[index].title;
                    do{
                        i=50;
                        while(i--){
                        getline(read,temp);
                        cout<<temp<<endl;
                        }
                        cout<<"\nEnter 1: for next page\n-1: to close the book\n :";
                        cin>>choice2;
                    }
                    while(choice2==1);
                    read.close();
                }
                else{
                    cout<<"Invalid book type input. Try again.\n";
                }
                break;
            case 4: 
                cout<<"Enter the type of book for which you want to analyse:\nEnter N for a Novel or P for a Play\n";
                cin>>ty;
                if(ty=='p'||ty=='P'){
                    cout<<"Enter the Id of the play that you want to analyse: ";
                    cin>>index;
                    if(index>=totalPlays){
                        cout<<"Invalid Input.Check the Id with search or display all once again.\n";
                        break;
                    }
                    cout<<"Enter the name of the character that you want to search for\n :";
                    cin>>word;
                    word=upCase(word);
                    word=" "+word;
                    read.open(path+"/"+plays[index].file_name);
                    read.seekg(plays[index].start);
                    map <int,set<string>> chc;
                    string act;
                    vector<string> scene;
                    set <string> ans;
                    string chars;
                    string ab="";
                    int sceneNo=0;
                    while(getline(read,temp)){
                        if(temp.find("ACT")!=string::npos){
                            act=temp;
                        }
                        if(temp.find("SCENE")!=string::npos){
                            scene.push_back(act+" "+temp);
                            sceneNo++;
                        }
                        istringstream x(temp);
                        x>>chars;
                        ab="";
                        if(std::all_of(chars.begin(),chars.end(),&::isupper)){
                            if(chars.compare("SCENE")==0||chars.compare("ACT")==0){}
                            else{
                                ab=ab+" "+chars;
                                x>>chars;
                            }
                        }
                        if(chars.back()=='.'){
                            chars.pop_back();
                            if(std::all_of(chars.begin(),chars.end(),&::isupper)){
                                ab=ab+" "+chars;
                                chc[sceneNo-1].insert(ab);
                            }
                        }
                    }
                    for(i=0;i<sceneNo;i++){
                        if(chc[i].find(word)!=chc[i].end()){
                            for(auto it=chc[i].begin();it!=chc[i].end();it++)
                            ans.insert(*it);
                        }
                    }
                    cout<<"The characters that share a scene with "<<word<<" are :\n";
                    for(auto it=ans.begin();it!=ans.end();it++){
                        cout<<*it<<endl;
                    }
                    read.close();
                }
                else if(ty=='n'||ty=='N'){
                    cout<<"Enter the Id of the novel that you want to analyse: ";
                    cin>>index;
                    if(index>=totalNovels){
                        cout<<"Invalid Input.Check the Id with search or display all once again.\n";
                        break;
                    }
                    cout<<"Enter the word that you want to search for: ";
                    cin>>word;
                    read.open(path+"/"+novels[index].file_name);
                    read.seekg(novels[index].start);
                    temp="";
                    int c=0,p=0;
                    string prev;
                    priority_queue <pair<int,string>> chap;
                    priority_queue <pair<int,string>> para;
                    while(read){
                        getline(read,temp);
                        n=temp.length();
                        k=word.length();
                        n-=k;
                        p=0;
                        for(int l=0;l<n;l++){
                            if(temp.compare(l,k,word)==0){
                                    c++;
                                    p++;
                            }
                        }
                        if(temp.find("CHAPTER")!=string::npos){
                            if(chap.size()>6){
                                chap.pop();
                            }
                            if(temp.find("CHAPTER 1")!=string::npos){
                                prev=temp;
                            }
                            else{
                                chap.push(make_pair(-c,prev));
                                c=0;
                                prev=temp;
                            }
                        }
                        para.push(make_pair(-p,temp));
                        if(para.size()>6){
                            para.pop();
                        }
                    }
                    cout<<"Do you want the paragraph or the chapter with the top 5 frquency.\nEnter P for Paragraph or C for Chapter\n :";
                    char ty2;
                    cin>>ty2;
                    if(ty2=='p'||ty2=='P'){
                        while(para.size()>5){
                            para.pop();
                        }
                        i=5;
                        while(i--){
                            cout<<"At rank "<<i+1<<": \n"<<para.top().second<<"\nwith "<<-para.top().first<<" repetitions of "<<word<<endl;
                            para.pop();
                        }
                    }
                    else if(ty2=='C'||ty2=='c'){
                        chap.pop();
                        i=5;
                        while(i--){
                            cout<<"At rank "<<i+1<<": \n"<<chap.top().second<<"\nwith "<<-chap.top().first<<" repetitions of "<<word<<endl;
                            chap.pop();
                        }
                    }
                    else{
                        cout<<"Invalid Input. Try again\n";
                    }
                    read.close();
                }
                else{
                    cout<<"Invalid book type input. Try again.\n";
                }
                break;
            case -1: 
            cout<<"Have a nice day!\n";
            break;
            default:
            cout<<"Invalid input. Try again\n";
            break;
        }
    }
    while(choice!=-1);

    
    return 0;
}