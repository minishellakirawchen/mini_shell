# Input
```c
char *input_line = readline("minishell > ");

$> minishell > hogehoge          -> input=[hogehoge]
$> minishell > "playground   "         -> input=["playground   "]
$> minishell > "hello " ' world '-> input=["hello " ' world ']
```

# Analysis
* |
* <, <<, >, >>
* ()
* &&, ||

### わからないこと
* <,<<,>,>>がある時の処理の流れが曖昧？

### わかっていること
* pipe単独であれば | で分割して、順番にfork, 実行すればOK
* &&, ||はpipeの前段階で分割してグルーピングする -> もう一段階階層が必要か？ -> 木構造で対応できそう
* () 実行時にForkして、その中で上記の実行をすれば良い、ただし実行順序の整合性は自信なし -> 木構造で対応できそう
* pipe, ()が混在した時の実行順が曖昧？ -> 木構造で対応できそう
* 分割する順番と粒度 -> 木構造で対応できそう
* 適切なparameterの持ち方 -> 木構造で対応できそう？
* 実行方法と実行順序 -> 木構造で対応できそう

<br>
<br> 

### Separate -> 汎用性低いためNG
```c
      input                         separated
[echo hello world]          -> {"echo" "hello" "world"}
[echo hello       world]    -> {"echo" "hello" "world"}

// まずpipeで分割する？
[cat Makefile | grep a]     -> {"cat" "Makefile"}, {"grep", "a"} //A
                               {"cat Makefile", "grep a"}        //B
```
<br>

### idea -> 汎用性低いためNG
1. separate by pipe and redirect
2. make pipe structure and assignment " " separated one 
```c
[cat Makefile | grep a]
-> {"cat Makefile", NULL}, {"grep a", NULL}, NULL
-> pipe[0] {"cat", "Makefile", NULL}, NULL
   pipe[1] {"grep", "a", NULL}, NULL
	
[cat Makefile | grep a | grep b | grep c | grep d]
-> {"cat Makefile", NULL}, {"grep a", NULL}, {"grep b", NULL}, {"grep c", NULL}, {"grep d", NULL}, NULL
-> pipe[0] {"cat", "Makefile", NULL}, NULL
   pipe[1] {"grep", "a", NULL}, NULL
   pipe[2] {"grep", "b", NULL}, NULL
   pipe[3] {"grep", "c", NULL}, NULL
   pipe[4] {"grep", "d", NULL}, NULL

[cat Makefile | grep a | grep b > outfile]
-> {"cat Makefile", NULL}, {"grep a", NULL}, {"grep b", NULL}, {">" "outfile", NULL}, NULL
-> pipe[0] {"cat", "Makefile", NULL}, NULL
   pipe[1] {"grep", "a", NULL}, NULL
   pipe[2] {"grep", "b", NULL}, NULL
   pipe[3] {">", "c", NULL}, NULL //TODO: implement for execute ">" to redirect out

[cat < infile | grep a | grep b > outfile]
-> {"cat", NULL}, {"<", "infile", NULL}, {"grep a", NULL}, {"grep b", NULL}, {">",  "outfile", NULL}, NULL
-> {"<", "infile", NULL}, {"cat", NULL}, {"grep a", NULL}, {"grep b", NULL}, {">",  "outfile", NULL}, NULL // swap <
-> pipe[0] {"<", "infile", NULL}, NULL  //TODO: implement for execute "<" to redirect in
   pipe[1] {"cat", NULL}, NULL
   pipe[2] {"grep", "a", NULL}, NULL
   pipe[3] {"grep", "b", NULL}, NULL
   pipe[4] {">", "outfile", NULL}, NULL //TODO: implement for execute ">" to redirect out
// <が鬼門？ 


// case of pipe and ()
[(cd /bin) | cat Makefile | grep a]
-> {"(cd /bin)", NULL}, {"cat Makefile", NULL}, {"grep a", NULL}, NULL
-> pipe[0] sub[0] {"cd /bin", NULL},
   pipe[1]        {"cat", "Makefile", NULL},
   pipe[2]        {"grep", "a", NULL}
// () は && 無しだと有り難みがない？

// もしくは...
-> {"cd",  "/bin", NULL}, {"cat Makefile", NULL}, {"grep a", NULL}, NULL // ()単独なら展開できる？
-> pipe[0] {"cd /bin", NULL},
   pipe[1] {"cat", "Makefile", NULL},
   pipe[2] {"grep", "a", NULL}

```
<br>

### 木構造にすると良さそう？ -> 1/6th 良さげ、しかしand, orへの対応に難あり？
* root = group : and, or > pipe > subshell(内部のgroupも同様に展開) -> and, orはstackに入れる？このままでよさそう。ORの中断をどうする？？これはStackがやりやすい
* leaf = command : char **cmds = {"cmd", "op",.., NULL}
* edge = 展開順
```c
# 木構造とする場合
[echo hello]
{"echo", "hello", NULL}


[echo hello | grep o]
[pipe]
 |________________________________
 |                               |
{"echo", "hello", NULL}       {"grep", "o", NULL}

[cat Makefile | grep a | grep b | grep c]
[pipe] |で結合する4要素を以下のように持ちたい
 |_______________________________________________________________________________________
 |                               |                           |                          |
{"echo", "hello", NULL}       {"grep", "a", NULL}           {"grep", "b", NULL}       {"grep", "c", NULL}


[cat Makefile | grep a && echo hello]
[and]
 |________________________________________________________________
 |                                                               |
[pipe]                                                          {"echo", "hello", NULL}
 |________________________________
 |                               |
{"cat", "Makefile", NULL}       {"grep", "a", NULL}


[pwd && (cd /bin && pwd) && pwd | cat -e] //一見良いが, A && B || Cに対応不可
[and]
 |___________________________________________________________
 |                   |                                      |
 {"pwd", NULL}      [sub]                                  [pipe]
                     |                                      |__________________________
                    [and]                                   |                         |
                     |____________________________         {"pwd", NULL}             {"cat", "-e", NULL}
					 |                           |
                    {"cd", "/bin", NULL}        {"pwd", NULL}

# Stackに積む場合 -> &&, ||で結合している要素がわかりにくい
input[echo hello]
stack[{echo hello}]

input[echo hello | grep a]
stack[{echo hello | grep a}]

input[echo hello | (cat Makefile | grep a)]
stack[{echo hello | (cat Makefile | grep a)}]

input[pwd && (cd /bin && pwd) && pwd]
stack[{pwd}, {(cd /bin && pwd)}, pwd] //()内の&&はいつ分解する？？

# AND, ORの実行
flg=ANDの場合:DFSで実行して成否判定をANDしていき, 全てtrueで終了 or AND=0になったときにreturnする
flg=ORの場合 :DFSで実行して成否判定をORしていき, OR=1になったとき or 全てfalseで終了でreturnする



```
* 線形リストでも類似の実装ができそう
* void *contentを工夫する？
* -> void *contentはキャストしないと中身を取り出せないため面倒、専用のstructを作成する

<br>

### idea NEW! 1/7th
* and, orへの対応を検討
* 全体をspace splitして、&&, ||まで実行, を繰り返せばbonus対応できそう
* pipeまで分割したら木構造に入れる

```c
[1] input[pwd && (cd /bin && pwd) && pwd]
split " " 
split[pwd, &&, (cd/ bin && pwd), &&, pwd, NULL] //""や()内の&&, ||は残す

1. spllit[i] == "&&"まで i++ -> pwd &&まで見る
2. pwdを実行
3. 次の&& or ||まで split[i++] -> (cd /bin && pwd), &&
4. (cd /bin && pwd)を実行->subshell, &&の処理を再度実行する必要あり
5. 次の&& or ||まで split[i++] -> pwd, NULL
6. pwdを実行


[2] input[cat Makefile | grep a | grep b && pwd && (cd /bin && touch file) && echo "hoge | hoge &&"]

split[cat,Makefile,|,grep,a,|,grep,b,&&,pwd,&&,(cd /bin && touch file),&&,echo,"hoge | hoge &&",NULL]

1. 1つめの&&, ||まで実行
  1-1:検索[cat,Makefile,|,grep,a,|,grep,b]
  1-2:pipeで分割してtreeに入れる
  1-3:順番に実行 {cat,Makefile,NULL}{grep,a, NULL},{grep,b,NULL}
  1-4:exit_statusを保持
  
2. 2つめの&&, ||まで実行
  2-1:検索[pwd]
   ...
3. 3つめの..
  3-1: (cd /bin && touch file)

4. 4つめ
  4-1: echo,"hoge | hoge &&"
  
  
```
* "", '', ()内のandを除外するsplitの実装が面倒すぎ
* 一旦pipeのみ（mandatory）で進めるところまで進む

<br>

### redirection
* pipexはredirection実行順が決まっていた && "< in cmd", "cmd > out"の順番だったため、順番に実行すれば流れができた
* これをどうするか？"< in cmd" == "cmd < in"
* あとは cmd > via1 > via2 > outなど、複数処理への対応
* redirectへの渡し方、実行方法、関数

```c

< file cmd, cmd < file // file -> cmd
1. fd = open(file, READ)
2. dup2(fd, STDOUT)
3. exec cmd

cmd > file, > file cmd
1. fd = open(file, WRITE)
2. dup2(fd, STDIN)
3. exec cmd



```

<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

以下、草案

### &&, ||のあるケース -> 一旦 pipe & ()のみ考える
```c
[cat Makefile | grep a && echo hello world && ls > outfile]
// cat Makefile | grep a, echo hello world, ls > outfileを順番に実行する
-> {"cat Makefile | grep a", "echo hello world", "ls > outfile", NULL}
-> and[0] pipe[0] {"cat Makefile", NULL},
   and[0] pipe[1] {"grep a", NULL},
   and[1] pipe[0] {"echo hello world", NULL},
   and[2] pipe[0] {"ls", NULL},
   and[2] pipe[1] {">" "outfile", NULL}

[pwd && (cd /bin && pwd) && pwd]
-> {"pwd", "(cd /bin && pwd)", "pwd"}
-> and[0]        pipe[0] {"pwd", NULL},
   and[1] and[0] sub [0] {"cd /bin", NULL},
   and[1] and[1] sub [0] {"pwd", NULL},
   and[2]        pipe[0] {"pwd", NULL},
// 入れ子が増えた時の対処方法... and[1] and[1] sub[1] or[2] ... 無限に増えそう
//前から実行して、状況に応じて再帰で実行すれば、上手く実行できそう？
-> 一旦 pipe & ()のみ考える
```












<br>


同じ意味
1) "cat < infile"
2) "< infile cat"

infileをstdinと繋げてcommandへ渡すなら 2)の方が扱いやすいため、"cat < infile"は"< infile cat"として実行したい
順番を入れ替える場合、問題となるケースがあるはず...

```c
cat < infile > via1 > via2 > outfile
< infile cat > via1 > via2 > outfile //これはOK


cat < via1 < via2 < infile

```

<br>

```c
[cat Makefile | grep && ]


```













<br>

### Make struct

# Expansion



# Execution
### 使用関数
```c
execve(char *path, char **cmd, char *env);
```

### パラメータの状態
```c
char *path = "/bin/ls";
char **cmd = {"/bin/ls", "-l", NULL};
char *env  = environ;
```
もしinputされたpathがファイル名であれば、env PATHより実行可能なpathを探す
```c
void ft_execvp(char *file, char **cmd, char *env, char **paths);
{
	char     *path;
	size_t  i;
	
	i = 0;
    while (*paths[i])
    {
        path = strjoin(paths[i], file);
		execve(path, **cmd, *env); // if execve success, not return
		free(path);
		i++;
    }
	printf("command not found");
}
```