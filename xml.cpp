/* astronaut xml */

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>


using namespace std;

/* attribute value struct */
class value
{
    enum VALUE_TYPE { VALUE_STRING, VALUE_INT, VALUE_NUMBER };

public:
    value(string s) { mType = VALUE_STRING; this->str = s; }
    value(int i) { mType = VALUE_INT; this->integer = i; }
    value(float n) { mType = VALUE_NUMBER; this->number = n; }
    ~value() {}

    string toString() { 
        switch (mType) { 
            case VALUE_STRING: return str;
            case VALUE_INT: return "integer";
            case VALUE_NUMBER: return "number";
            default: return "no value";
        }
    }
private:
    int mType;
    string  str;
    int     integer;
    float   number;
};

/* XMLBlock class */
class XMLBlock
{
public:
    XMLBlock(string tag_name, string content) 
    {
        mTagName = tag_name;
        mContent = content;
    }
    XMLBlock(string tag_name)
    {
        mTagName = tag_name;
        mContent = "";
    }

    ~XMLBlock();

    void AddAttr(string attr_name, value attribute) 
    {
        mAttributes.insert(pair<string, value>(attr_name, attribute));
    }

    void AddChild(XMLBlock* block)
    {
        mChildren.push_back(block);
    }

    string GetName()
    {
        return mTagName;
    }
protected:
    string              mTagName;
    string              mContent;
    map<string, value>  mAttributes;
    vector<XMLBlock*>   mChildren;
};

// function declarations
int ParseFile(const char* buffer, int size);
int ParseFile(const char* filename);

// main
int main(int argc, char* argv[])
{
    const char* file = "index.html";
    if (argc > 1) file = argv[1];

    int xml = ParseFile(file);

    if (xml) {
        return 0;
    }

    return 1;
}

int ParseText(const char* buffer, int size)
{
/*  scan for <> tags with attributes
    then create an XML object containing
    the internal data 

    look for </> closing tags that match
    to move up the stack
*/
    vector<XMLBlock*>   parsed;
    vector<XMLBlock*>   blocks;

    // parsing variables
    unsigned int seek_a, seek_b;
    unsigned int inner_a, inner_b;
    int token;
    bool parsing;
    int result;
    int level = 0;

    bool matching_content = false;
    char* matching_tag = NULL;

    // init parser state
    seek_a = seek_b = 0;
    token = 0;
    parsing = true;
    //root = NULL;
    //level = NULL;

    inner_a = inner_b = 0;

    // parse input
    while (parsing &&
           seek_a < size) 
    {
        // locate beginning of tag
        if (buffer[seek_a] == '<') {
            // check for content between tags
            if (inner_a < seek_a) inner_b = seek_a;

            // block comments
            if ((seek_a + 3) < size &&
                buffer[seek_a+1] == '!' &&
                buffer[seek_a+2] == '-' &&
                buffer[seek_a+3] == '-')
            {
                cout << "<!-- -->" << endl;
                seek_a = seek_a + 4;
                while ((seek_a + 2) < size &&
                        buffer[seek_a] != '-' &&
                        buffer[seek_a+1] != '-' &&
                        buffer[seek_a+2] != '>')
                {
                    seek_a++;
                }
                seek_a = seek_a + 2;
                continue;
            }

            bool closing, self_closing;
            seek_b = seek_a;

            // check for closing tag
            closing = ((seek_a+1) < size && buffer[seek_a+1] == '/');
            
            // find tag closing brace
            while (seek_b < size &&
                   buffer[seek_b] != '>')
            {
                seek_b++;
            }

            // check for self-closing tag
            if (seek_b && buffer[seek_b-1] == '/') {
                self_closing = true;
            } else {
                self_closing = false;
                //cout << "normal" << endl;
            }

            // locate end of tag
            if (seek_b < size &&
                buffer[seek_b] == '>')
            {
                // complete tag is between (a and b)
                unsigned int text_a, text_b;
                text_a = text_b = ((seek_a + 1) + (closing ? 1 : 0));
                while (text_b < size &&
                       buffer[text_b] != ' ' &&
                       buffer[text_b] != '>')
                {
                    text_b++;
                }

                // tag name is between text_a and text_b
                char* name;
                unsigned int len;
                int i;

                len = text_b - text_a;
                name = new char[len+1];
                
                // copy string
                for (i = 0; i < len; i++)
                {
                    name[i] = buffer[text_a + i];
                }
                name[len] = '\0';

                // attr is between text_b and seek_b

                // scan over tags like <script> and <style>
                if (matching_content && 
                       (!closing ||
                        strcmp(name, matching_tag) != 0))
                {
                    seek_a++;
                    continue;
                }

                // content of previous tag
                if ((inner_b - inner_a) > 0 && inner_b < size && inner_b > inner_a) 
                {
                    char* content;
                    unsigned int content_len;
                    unsigned int j;

                    content_len = inner_b - inner_a;
                    content = new char[content_len+1];
                    for (j = 0; j < content_len; j++)
                        content[j] = buffer[inner_a + j];
                    content[content_len] = '\0';

                    cout << "content: '" << content << "'" << endl;
                }

                // check for opening 
                // or closing tag
                if (closing) {
                    // closing tag

                    level--;

                    for (i = 0; i < level; i++)
                        cout << "  ";
                    if (level < 0) {
                        cout << "error: mismatched tags." << endl;
                        return 0;
                    }

                    cout << "<" << "/" << name << ">" << endl;

                    if (matching_content) {
                        matching_content = false;
                        matching_tag = NULL;
                    }

                } else {
                    // opening tag
                    for (i = 0; i < level; i++)
                        cout << "  ";
                    cout << "<" << name << ">" << endl;

                    if (!self_closing) level++;

                    if (strcmp(name, "script") == 0 ||
                        strcmp(name, "style") == 0)
                    {
                        matching_content = true;
                        matching_tag = name;
                    } else {
                        matching_content = false;
                        matching_tag = NULL;
                    }
                }

                cout << "attr: '"; 
                for (i = text_b; i < seek_b - 1 || (buffer[seek_b-1] != '/' && i < seek_b); i++) {
                    cout << buffer[i]; 
                }
                cout << "'" << endl;

                seek_a = seek_b + 1;
                inner_a = inner_b = seek_a;

                // add to object

            } else {
                // end of input, error

                return 0;
            }
        }
        else
        {
            seek_a++;
        }
    }

    return 1;
}

// ParseFile
int ParseFile(const char* filename)
{
    char* Text;
    ifstream file;
    unsigned int size;
    streampos length;

    /* load file into memory */
    file.open(filename, ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
        length = file.tellg();
        Text = new char[(int)length+1];

        file.seekg(0, ios::beg);
        file.read(Text, length);
        file.close();
        Text[length] = '\0';

        size = (int)length + 1;

        /* parse text data by tags */
        cout << filename << endl;
        cout << length << endl;
        int result = ParseText(Text, size);

        /* free resources */
        delete[] Text;
        return result;
    }
    return 0;
}

