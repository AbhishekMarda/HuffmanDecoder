//
//  decoder.cpp
//  huffmandecoder
//
//  Created by Abhishek Marda on 6/14/20.
//  Copyright © 2020 Abhishek Marda. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include "characternode.h"

using namespace std;


// Input will be in the form
 /*
 char1 encoding1
 char2 encoding2
 ...
 charN encodingN
 
 
 remainderTerm
 
 encoding of file
 */

//alternate approach, use a hashmap to store the encoding of every characters and keep checking while traversing a string

void addNode(CharacterNode* &root, string bitpattern, const char character);
void printLetter(CharacterNode* root, string code);
void cleanupBinaryTree (CharacterNode* root);
void processByte(string& output, const char byte, CharacterNode* &currptr, CharacterNode* root, int remainder);

int main(int argc, const char * argv[]) {
    if (argc!=3)
    {
        cout<<"Please enter your encoded file spaced by the name of the .txt file you want as output."<<endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        cout <<"Problem with the input file. Please try again."<<endl;
        return 1;
    }

    string line;
    CharacterNode* root=nullptr;
    while (getline(inputFile,line))
    {
        if (line=="") //this could mean the end of the characters entry or the input of a newline character. This code block handles the difference between the two
        {
            getline(inputFile, line);
            if (line == "") break; //a second empty line means that it's the end of the characters
            char c = '\n';
            string charEncoding = line;
            addNode(root, charEncoding, c);
            continue;
        }
        char c = line[0];
        string charEncoding = line.substr(2);

        addNode(root, charEncoding, c);
    }
    
    //next line would contain the remainder
    getline(inputFile, line);
    short remainder = line[0]-'0';
    
    
    getline(inputFile, line);
    if (line!="")
    {
        cout<<"Check failed."<<endl;       //friendly check
    }
    string encoding;
    getline(inputFile, line);
    encoding+=line;
    while(getline(inputFile, line)) //still require a loop in case the ascii interpretation of a 8-bit pattern is a newline
    {
        encoding+='\n'+line;
    }
    string output;
    CharacterNode* currptr=root;
    for (int i=0; i<encoding.length()-1; ++i)
    {
        processByte(output, encoding[i], currptr, root,0);
    }
    //we need to process the last byte a little differently
    processByte(output, encoding[encoding.length()-1], currptr, root, 8-remainder);
    
    string outputfilename = argv[2];
    outputfilename+=".txt";
    ofstream outputFile (outputfilename);
    
    outputFile << output;
    cleanupBinaryTree(root);
    
}
///Replicate the placement of a character in a binary tree using its corresponding bit pattern from the encoded file.
void addNode(CharacterNode* &root, string bitpattern, const char character)
{
    CharacterNode* currptr = root;
    while(bitpattern.length()!=0)
    {
        if (currptr == nullptr)     //happens only in the very first case
        {
            currptr= new CharacterNode;
            root = currptr;
        }
        char bit = bitpattern[0];
        switch (bit)
        {
            case '0':
                if (currptr->m_left==nullptr)
                    currptr->m_left = new CharacterNode;
                currptr = currptr->m_left;
                break;
            case '1':
                if (currptr->m_right==nullptr)
                    currptr->m_right = new CharacterNode;
                currptr = currptr->m_right;
            default:
                break;
        }
        if (bitpattern.length()==1)
            currptr->m_char=character;
        bitpattern = bitpattern.substr(1);
    }
    
}

///Useful for debugging by printing the binary tree representation given root pointer
void printLetter(CharacterNode* root, string code)
{
    if (root==nullptr)
        return;
    if (root->m_char!=0)
    {
        cout<<root->m_char<<" "<< code<<endl;
        return;
    }
    printLetter(root->m_left, code + "0");
    printLetter(root->m_right, code + "1");
    
}

///Delete binary tree from the heap
void cleanupBinaryTree(CharacterNode* root)
{
    if (root == nullptr)
        return;
    cleanupBinaryTree(root->m_left);
    cleanupBinaryTree(root->m_right);
    delete root;
    return;
}

/**
 Processes a single encoded byte and appends it to a string that contains the decoding. This function should be used in a while loop since it continuously keeps track of the traversal pointer.
 */
void processByte(string& output, const char byte, CharacterNode* &currptr, CharacterNode* root, int remainder)
{
    char basemask = 1;
    for(int i=7; i>=0+remainder; --i)
    {
        char mask = basemask<<i;
        char masked = byte & mask;
        masked = (masked >> i)&0b1; //cute star wars joke
        switch(masked)
        {
            case 1: currptr = currptr->m_right;  break;
            case 0: currptr = currptr->m_left;  break;
            default: cerr<<"You shouldn't have reached here"<<endl;
        }
        if (currptr->m_char!=0)
        {
            output+=currptr->m_char;
            currptr = root;
        }
        
    }
}
