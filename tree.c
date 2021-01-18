#include <stdio.h>
#include <stdlib.h>
#define NUM_LETTERS ((int)26)

typedef enum {
    FALSE=0,
    TRUE=1
} boolean;

typedef struct node {
    char letter;//the char that represented by this node
    long unsigned int count;//count how many words ended with this node
    struct node* children[NUM_LETTERS];
    struct node * parent;//the node that lead to the creation of this node
    int counterPath;//count how many words using this nodes
} node;
//function that received char and parent and then create a new node
node * newNode(char ch,node * p)
{
    node * n=(node*)malloc(sizeof(node));
    if (n == NULL)
    {
        printf("malloc for new node failed\n");
        return NULL;
    }
    n->letter=ch;
    n->count=0;
    //for loop that initialize all the children of this node to null
    for(int i=0; i<NUM_LETTERS;i++)
        (n->children)[i]=NULL;
    n->parent=p;
    n->counterPath=0;
    return n;
}
//function that gets pointer to the root of the tree and delete the tree
void deleteTree(node * root)
{
    if(root==NULL)
        return;
    //delete all of his children
    for(int i=0;i<NUM_LETTERS;i++)
        deleteTree(root->children[i]);
    free(root);
}
//function that gets char and transform it to lower case char if it not letter return !
char lowerCaseLetter(char ch)
{
    if(ch>='A'&&ch<='Z')
        return ch-'A'+'a';
    if(ch>='a'&&ch<='z')
        return ch;
    return '!';

}
//function that read txt from the standard input and build Trie tree for it
// if succeed return the root else return null
node* fill_tree()
{
    node *root=newNode('!', NULL);//creating root
    node *current=root;//pointer that used to running on the tree
    node *new_node=NULL;
    if (!root)
    {
        printf("malloc for root failed");
        return NULL;
    }
    char ch;
    boolean spaceFlag=TRUE;//boolean that prevent \n and space that are not in the end of a word
    while(scanf("%c", &ch)==1 )
    {
        //checks if reached to a end of a word
        if(ch==' '|| ch=='\n')
        {
            //checks if its a end of a word or sequence of \n or spaces
            if(spaceFlag == TRUE)
            {
                (current->count)+=1;//the word ended on this node so count gets +1
                spaceFlag=FALSE;//to prevent sequences
            }
            current=root;//its a end of word so return current to the root
            continue;

        }
        ch=lowerCaseLetter(ch);//if ch is a letter transform it to lower case else return !
        if (ch=='!')//ch wasn't a letter so ignore it and moves to the next char
            continue;
        spaceFlag=TRUE;//we successfully read a letter so we can it cant be a sequence
        //checks if this char creating a new route in Trie
        if(current->children[ch-'a'] == NULL)
        {
            new_node=newNode(ch,current);
            if(new_node==NULL)
            {
                printf("malloc for new node failed\n");
                deleteTree(root);
                return NULL;
            }
            current->children[ch-'a'] =new_node;//connect it to the Trie
            current=current->children[ch-'a'];//forwarding current
            current->counterPath += 1;//its part of a word so counterPath get +1
        }
        //its not a new route
        else
            {
            current = current->children[ch - 'a'];//forwarding current
            current->counterPath += 1;//its part of a word so counterPath get +1
        }
    }
    //if the txt is not ending with space or \n
    if(ch !=' '&&ch !='\n')
        current->count+=1;
    return root;
}

//function that gets pointer to node and the number of words that end with this node
//and subtract this num from the counterPath of each node in the route of this word
void removeCounter(node * node_ptr,int num)
{
    node * temp=node_ptr;
    while(temp->parent!=NULL)
    {
        temp->counterPath-=num;
        temp=temp->parent;
    }
}
//function that gets root of a Trie and boolean that symbol in which way we need to print the graph
//from high to low lexicography order or from low to high
void printFromHigh(node * root,boolean which_order)
{
    //stop condition
    if(root==NULL)
        return;
    node * current = root;
    boolean not_end_of_word=TRUE;
    boolean is_SubString=FALSE;
    boolean fromHigh = which_order;
    while(not_end_of_word == TRUE)
    {
        //if this word is substring of other word it need to be print before (high to low )
        if(fromHigh==TRUE)
            if(current->count > 0)
            {
                printf(" %ld\n", current->count);
//                if((current->count) >= 1)
                    removeCounter(current, (current->count));
                //ezer->counterPath -=(ezer->count);
                current->count=0;
                is_SubString=TRUE;
                break;
            }
        not_end_of_word=FALSE;
        if(fromHigh==TRUE)
        {
            //running on all of the children and searching for the smallest letter that its counter path is not zero
            for(int i=0;i<NUM_LETTERS;i++)
                if(current->children[i] != NULL && (current->children[i])->counterPath != 0)
                {
                    not_end_of_word=TRUE;//didnt finish to read the word so return on the while
                    current=current->children[i];
                    printf("%c", current->letter);
                    break;
                }
        }
        else
            // running on all of the children and searching for the biggest letter that its counter path is not zero
            for(int i=NUM_LETTERS-1;i>=0;i--)
            {
                if(current->children[i] != NULL && (current->children[i])->counterPath != 0)
                {
                    not_end_of_word=TRUE;
                    current=current->children[i];
                    printf("%c", current->letter);
                    break;
                }
            }
    }
    //if its end of a word and its not a substring of another word
    if(current->count != 0 && is_SubString == FALSE)
    {
        printf(" %ld\n", current->count);
//        if((current->count) >= 1)
            removeCounter(current, (current->count));
        current->count=0;
    }
    //
//    if(is_SubString == FALSE)
    {
        node * p=current->parent;
        if(p==NULL)
            root=NULL;
    }
    return printFromHigh(root,fromHigh);
}

int main(int argc,char * argv[])
{
    node * root =fill_tree();
    if(root == NULL)
    {
        printf("a Trie is no created");
        return -1;
    }
    if(argc==1)
        printFromHigh(root,TRUE);
    else if (*argv[1]=='r')
        printFromHigh(root,FALSE);
    deleteTree(root);
}
//free(n);