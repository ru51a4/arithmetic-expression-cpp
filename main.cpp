#include <memory>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class node
{
public:
  string token;
  shared_ptr<node> next;
  shared_ptr<node> nextnext;
  ~node() { cout << "token(" << this->token << ")" << " del, shared_ptr work \n"; }
};

string add(string a, string b)
{
  int c = stoi(a) + stoi(b);
  return to_string(c);
};

string _minus(string a, string b)
{
  int c = stoi(a) - stoi(b);
  return to_string(c);
};

string _multiplication(string a, string b)
{
  int c = stoi(a) * stoi(b);
  return to_string(c);
};

string _div(string a, string b)
{
  int c = stoi(a) / stoi(b);
  return to_string(c);
};

int main()
{
  // INPUT
  string str = "(1+2+3*(3+5)+4+5)*10";
  string r_str = str;
  //

  auto next = [&str]()
  {
    while (true)
    {
      char ch = str.front();
      str.erase(0, 1);
      if (ch != ' ' && ch != '\n' && ch != '\t')
      {
        return ch;
      }
    }
  };
  string s = "";
  vector<shared_ptr<node>> stack;
  shared_ptr<node> r = make_shared<node>();
  shared_ptr<node> res = r;
  while (str.size() > 0)
  {
    char ch = next();

    if (ch == '(')
    {
      shared_ptr<node> _t = make_shared<node>();
      s = "";
      shared_ptr<node> t = make_shared<node>();
      t->nextnext = _t;
      r->next = t;
      r = _t;
      stack.push_back(t);
    }
    else if (ch == ')')
    {
      r->next = make_shared<node>();
      r->next->token = s;
      s = "";
      r = stack.back();
      stack.pop_back();
    }
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
    {
      if (size(s) > 0)
      {
        r->next = make_shared<node>();
        r->next->token = s;
        r = r->next;
      }
      //
      r->next = make_shared<node>();
      r->next->token = ch;
      r = r->next;
      //
      s = "";
    }
    else
    {
      s = s + ch;
    }
  }

  if (size(s) > 0)
  {
    r->next = make_shared<node>();
    r->next->token = s;
    r = r->next;
  }

  function<shared_ptr<node>(shared_ptr<node>)> build = [&](shared_ptr<node> curr) -> shared_ptr<node>
  {
    shared_ptr<node> _curr = curr;
    int c = 0;
    while (curr)
    {
      if (curr->nextnext)
      {
        curr->token = (build(curr->nextnext))->token;
      }
      curr = curr->next;
    }
    shared_ptr<node> curr_p;
    //
    curr = _curr;
    while (curr)
    {
      if (curr->next && curr->next->next && curr->next->token == "*")
      {
        curr->token = _multiplication(curr->token, curr->next->next->token);
        curr->next = curr->next->next->next;
      }
      else if (curr->next && curr->next->next && curr->next->token == "/")
      {
        curr->token = _div(curr->token, curr->next->next->token);
        curr->next = curr->next->next->next;
      }
      else
      {
        if (!curr->next)
        {
          curr_p = curr;
        }
        curr = curr->next;
      }
    }
    //
    curr = _curr;
    while (curr)
    {
      if (curr->next && curr->next->next && curr->next->token == "+")
      {
        curr->token = add(curr->token, curr->next->next->token);
        curr->next = curr->next->next->next;
      }
      else if (curr->next && curr->next->next && curr->next->token == "-")
      {
        curr->token = _minus(curr->token, curr->next->next->token);
        curr->next = curr->next->next->next;
      }
      else
      {
        if (!curr->next)
        {
          curr_p = curr;
        }
        curr = curr->next;
      }
    }
    return curr_p;
  };

  string result = build(res)->token;
  cout << "\n"
       << r_str << " = " << result << "\n"
       << "\n";

  return 0;
}
