#include <bits/stdc++.h>

using namespace std;

// node structure
struct node
{
   int *data;        // for store value
   node **child_ptr; // children pointer
   int n;            // number of stored value
   bool id;          // for index node, id=false & for data node, id=true
   node *parent;     // for parent of node
};

node *root = nullptr;
int index_nodes_num = 0;
int leaf_nodes_num = 0;

// index node
node *init_I(int t)
{
   node *np = new node;
   np->data = new int[2 * t + 1];
   np->child_ptr = new node *[2 * t + 2];
   np->n = 0;
   np->id = false;
   for (int i = 0; i < 2 * t + 2; i++) // for children
   {
      np->child_ptr[i] = nullptr;
   }
   np->parent = nullptr;
   index_nodes_num++;
   return np;
}

// leaf node
node *init_L(int d)
{
   node *np = new node;
   np->data = new int[2 * d];
   np->id = true;
   np->child_ptr = new node *[2]; // for left or right sibling
   np->n = 0;
   for (int i = 0; i < 2; i++)
   {
      np->child_ptr[i] = nullptr;
   }
   np->parent = nullptr;
   leaf_nodes_num++;
   return np;
}

// split index node
void InsertionInBTree(node *curr, node *newptr, int separtor, int t)
{
   if (curr->parent == nullptr)
   {
      // here no parent node  so create node
      node *temp = init_I(t);
      temp->child_ptr[0] = curr;
      temp->child_ptr[1] = newptr;
      temp->data[0] = separtor;
      (temp->n)++;
      curr->parent = temp;
      newptr->parent = temp;
      root = temp;
      return;
   }
   else
   {
      curr = curr->parent;
      if (curr->n < 2 * t + 1)
      {
         // index node has space
         int count = (curr->n);
         while (curr->data[count - 1] > separtor & count > 0)
         {
            // data k liye
            curr->data[count] = curr->data[count - 1];
            // child pointers k liye
            curr->child_ptr[count + 1] = curr->child_ptr[count];
            count--;
         }
         (curr->n)++;
         curr->data[count] = separtor;
         curr->child_ptr[count + 1] = newptr;
         newptr->parent = curr;
         return;
      }
      else
      {
         // spilt index node
         node *newptr2 = init_I(t);
         if (curr->data[t - 1] < separtor & separtor < curr->data[t])//curr is the index node to be spiltted
         {
            for (int i = 0; i < t + 1; i++)
            {
               newptr2->data[i] = curr->data[t + i];
               newptr2->child_ptr[i + 1] = curr->child_ptr[t + i + 1];
               curr->child_ptr[t + i + 1] = nullptr;
               (newptr2->child_ptr[i + 1])->parent = newptr2;
            }
            curr->n = t;
            newptr2->n = t + 1;
            newptr2->child_ptr[0] = newptr;
            newptr->parent = newptr2;
            InsertionInBTree(curr, newptr2, separtor, t);
         }
         else if (separtor > curr->data[t])
         {
            int separtor2 = curr->data[t];//earlier it was t+1
            newptr2->child_ptr[0] = curr->child_ptr[t + 1];
            (newptr2->child_ptr[0])->parent = newptr2;
            for (int i = 1; i < t + 1; i++)
            {
               newptr2->data[i - 1] = curr->data[t + i];
               newptr2->child_ptr[i] = curr->child_ptr[t + i + 1];
               curr->child_ptr[t + i + 1] = nullptr;
               (newptr2->child_ptr[i])->parent = newptr2;
            }
            int count = t;
            while (newptr2->data[count-1] > separtor & count>0)
            {
               //finding correct position for the separator to be inserted
               // data k liye
               newptr2->data[count] = newptr2->data[count - 1];
               // child pointers k liye
               newptr2->child_ptr[count + 1] = newptr2->child_ptr[count];

               count--;
            }
            newptr2->n = t + 1;
            curr->n = t;
            newptr2->data[count] = separtor;
            newptr2->child_ptr[count + 1] = newptr;
            newptr->parent = newptr2;
            InsertionInBTree(curr, newptr2, separtor2, t);
         }
         else
         {
            int separtor2 = curr->data[t - 1];
            newptr2->child_ptr[0] = curr->child_ptr[t];
            (newptr2->child_ptr[0])->parent = newptr2;
            for (int i = 0; i < t + 1; i++)
            {
               newptr2->data[i] = curr->data[t + i];
               newptr2->child_ptr[i+1] = curr->child_ptr[t + i + 1];
               curr->child_ptr[t + i + 1] = nullptr;
               (newptr2->child_ptr[i+1])->parent = newptr2;
            }
            int count = t-1;
            while (newptr2->data[count-1] > separtor & count >0)
            {
               // data k liye
               curr->data[count] = curr->data[count - 1];
               // child pointers k liye
               curr->child_ptr[count + 1] = curr->child_ptr[count];

               count--;
            }
            newptr2->n = t + 1;
            curr->n = t;
            curr->data[count] = separtor;
            curr->child_ptr[count + 1] = newptr;
            newptr->parent = curr;
            InsertionInBTree(curr, newptr2, separtor2, t);
         }
      }
   }
}

void insert(int value, int d, int t)
{
   node *curr;
   curr = root;
   if (curr == nullptr)
   {
      // create new node
      root = init_L(d);
      root->data[0] = value;
      (root->n)++;
      // curr = root;
   }
   else
   {
      // Goes to leaf which value stored
      while (curr->id == false)
      {
         if (value < curr->data[0])
         {
            curr = curr->child_ptr[0];
         }
         else if (value >= curr->data[(curr->n) - 1])
         {
            curr = curr->child_ptr[curr->n];
         }
         else
         {
            for (int i = 1; i < curr->n; i++)
            {
               if (value >= (curr->data[i - 1]) && value < (curr->data[i]))
               {
                  curr = curr->child_ptr[i];
                  break;// To come out from loop
               }
            }
         }
      }
      // if leaf node has space
      if (curr->n < 2 * d)
      {
         curr->data[curr->n] = value;
         (curr->n)++;
         sort(curr->data, curr->data + (curr->n));
      }
      else // split leaf node
      {
         int separtor;
         node *newptr = init_L(d);
         if (curr->data[d - 1] < value)
         {
            for (int i = 0; i < d; i++)
            {
               newptr->data[i] = curr->data[d + i];
            }
            newptr->data[d] = value;
            curr->n = d; // reduced size after splitting
            newptr->n = d + 1;
            sort(newptr->data, newptr->data + (newptr->n));
            separtor = newptr->data[0];
         }
         else
         {
            for (int i = 0; i < d + 1; i++)
            {
               newptr->data[i] = curr->data[d + i - 1];
            }
            curr->data[d - 1] = value;
            curr->n = d; // reduced size after splitting
            newptr->n = d + 1;
            sort(curr->data, curr->data + (curr->n));
            separtor = newptr->data[0];
         }
         node *right_sibling = curr->child_ptr[1];
         curr->child_ptr[1] = newptr;
         newptr->child_ptr[0] = curr;
         if (right_sibling != nullptr)
         {
            newptr->child_ptr[1] = right_sibling;
            right_sibling->child_ptr[0] = newptr;
         }
         // store separtor in index node
         InsertionInBTree(curr, newptr, separtor, t);//t is the parameter for index node
      }
   }
}

void display(int t, int d)
{
   node *curr;
   curr = root;
   cout << index_nodes_num << " " << leaf_nodes_num << " ";
   // while (curr->id == false)
   // {
   //    curr = curr->child_ptr[0];
   // }
   // cout << "Root : ";
   for (int i = 0; i < root->n; i++)
   {
      cout << root->data[i] << " ";
   }
   // cout << endl;
   // cout << " Leaf values : ";
   // while (curr != nullptr)
   // {
   //    for (int i = 0; i < curr->n; i++)
   //    {
   //       cout << curr->data[i] << " ";
   //    }
   //    cout << " | ";
   //    curr = curr->child_ptr[1];
   // }
   cout << endl;
}

int main()
{
   int t, d;
   cin >> d >> t;
   int choice;
   while (1)
   {
      cin >> choice;
      if (choice == 1)
      {
         // Insert
         int key;
         cin >> key;
         insert(key, d, t);
      }
      else if (choice == 2)
      {
         // cout << index_nodes_num << " " << leaf_nodes_num << endl;
         display(t, d);
      }
   }
}
