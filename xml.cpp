/* astronaut xml */

#include <iostream>
#include <fstream>
using namespace std;

// function declarations
//XMLObject* 
int ParseFile(const char* buffer, int size);
//XMLObject* 
int ParseFile(const char* filename);
/*
// list type
template <Type>
class ListEntry {

}

template <Type>
class List {
public:
    List();
    List(Type arr[], int length);

    int add(Type element) {
        last->next = 
    }
private:

};

// XMLObject type
class XMLObject 
{
public:
    XMLObject(const char* name, const char* attributes, bool self_closing)
    {
        mName = name;
        mContent = NULL;
        mAttributes = attributes;
        mSelfClosing = self_closing;

        parent = NULL;
        last = NULL;
        next = NULL;
        firstChild = NULL;
    }

    ~XMLObject() {
        if (mName) delete[] mName;
        if (mContent) delete[] mContent;
        if (mAttributes) delete[] mAttributes;
        if (firstChild) delete firstChild;
        if (next) delete next;
    }

    void add(const char* name, const char* attributes, bool self_closing) 
    {
        next = new XMLObject(name, attributes, self_closing);
        next->parent = parent;
        
    }
    bool tagIs(const char* tag);
    void close();
    XMLObject* next();
    XMLObject* parent();

private:
    const char* mName;
    const char* mContent;
    const char* mAttributes;

//  List<Attr>  mAttr;
    bool mSelfClosing;
    XMLObject* parent;
    XMLObject  *last, 
               *next;
    XMLObject* firstChild;
};
*/
/*
class XMLObject {
public:
    XMLObject(const char* name, const char* attributes, bool self_closing)
    {
        cout << name;
        if (self_closing) {
            cout << '\';
        }
        cout << endl;
    }
}
*/
// main
int main(int argc, char* argv[])
{
    //XMLObject* xml;
    int xml = ParseFile("index.html");

    if (xml) {
        //xml->Print();
        //delete xml;
        return 0;
    }

    return 1;
}

// ParseText
//XMLObject* 
int
ParseText(const char* buffer, int size)
{
/*  scan for <> tags with attributes
    then create an XML object containing
    the internal data 

    look for </> closing tags that match
    to move up the stack
*/
    // parsing variables
    unsigned int seek_a, seek_b;
    int token;
    bool parsing;
    //XMLObject* root;
    //XMLObject* level;
    int result;
    int level = 0;

    // init parser state
    seek_a = seek_b = 0;
    token = 0;
    parsing = true;
    //root = NULL;
    //level = NULL;

    // parse input
    while (parsing &&
           seek_a < size) 
    {
        //cout << "a: " << seek_a << endl;
        if (buffer[seek_a] == '<') {
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
                //cout << "self-closing" << endl;
            } else {
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
                unsigned int i;

                len = text_b - text_a;
                name = new char[len+1];
                
                //cout << "cpy name" << endl;
                // copy string
                for (i = 0; i < len; i++)
                {
                    name[i] = buffer[text_a + i];
                }
                name[len] = '\0';

                // attr is between text_b and seek_b
                /*
                char* attr;
                while (text_b < size && (buffer[text_b] == ' ' ||
                        buffer[text_b] == '/')) 
                {
                    text_b++;
                }
                len = seek_b - text_b;
                if (len) {
                    //cout << "cpy attr " << len << endl;
                    // copy attributes
                    for (i = 0; i < len; i++) {
                        attr[i] = buffer[text_b + i];
                    }
                    attr[len] = '\0';
                } else {
                    attr = NULL;
                }*/

                //cout << "name" << endl;
                // check for opening 
                // or closing tag
                if (closing) {
                    // closing tag
                    /*while (level) {
                        if (level->tagIs(name)) 
                        {
                            level->close();
                            level = level->next();
                        } else {
                            level = level->parent();
                        }
                    }*/
                    /*
                    if (level == NULL) {
                        // error with mismatched tags
                        //if (root) delete root;
                        return 0;
                    }*/

                    cout << "name: ";
                    //for (i = 0; i < level; i++)
                    //    cout << "  ";
                    //if (level < 0) {
                    //    cout << "error: mismatched tags." << endl;
                    //    return 0;
                    //}

                    cout << "/" << name << endl;
                    level--;
                } else {
                    // opening tag
                    /*if (self_closing) { }
                    // if tree has been initialized
                    if (root && level) {
                        level->add(name, attr, self_closing);
                        level = level->next();
                    } else {
                        root = new XMLObject(name, attr, self_closing);
                        level = root->next();
                    }*/
                    cout << "name: ";
                    //for (i = 0; i < level; i++)
                    //    cout << "  ";
                    cout << name << endl;
                    level++;
                }

                seek_a = seek_b + 1;
                // add to object
                //if (level) {
                //    result.add(name, attr, closing, self_closing);
                //}
                //else {
                //    result = new XMLObject(name, attr, closing, self_closing);
                //}
            } else {
                // end of input, error
                /*if (root) {
                    delete root;
                }*/
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
//XMLObject* 
int
ParseFile(const char* filename)
{
    char* Text;
    ifstream file;
    unsigned int size;
    streampos length;

    /* load file into memory */
    file.open(filename, ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
        length = file.tellg();
        Text = new char[length+1];

        file.seekg(0, ios::beg);
        file.read(Text, length);
        file.close();
        Text[length] = '\0';

        size = length + 1;

        /* parse text data by tags */
        //XMLObject* result;
        cout << filename << endl;
        cout << length << endl;
        int result = ParseText(Text, size);

        /* free resources */
        delete[] Text;
        return result;
    }
    return 0;
}

