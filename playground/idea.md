# Input
```c
char *readline_input = readline("minishell > ");

$> minishell > hogehoge          -> input=[hogehoge]
$> minishell > "playground   "   -> input=["playground   "]
$> minishell > "hello " ' world '-> input=["hello " ' world ']
```

# Expansion
### わかっていない・曖昧なこと
* varの定義 : manを確認する
* $var=echo, $> $var hello -> echo hello ?

### わかっていること
* executionに渡す前に展開しておいた方が、取り扱いやすい


### varの定義
* name=value
* value is optional
* variable parameter : ^$[_a-zA-z][_a-zA-z0-9]
* 

<br>
<br>

# Analysis
* |
* <, <<, >, >>
* ()
* &&, ||

### わかっていない・曖昧なこと
* bashの実装、より良い実装（一応できたっぽいが、改善点はたくさんあるはず）

### わかっていること 
* pipe単独であれば | で分割して、順番にfork, 実行すればOK
* &&, ||はpipeの前段階で分割してグルーピングする -> もう一段階階層が必要か？ -> 木構造で対応できそう
* () 実行時にForkして、その中で上記の実行をすれば良い、ただし実行順序の整合性は自信なし -> 木構造で対応できそう
* pipe, ()が混在した時の実行順が曖昧？ -> 木構造で対応できそう
* 分割する順番と粒度 -> 木構造で対応できそう
* 適切なparameterの持ち方 -> 木構造で対応できそう？
* 実行方法と実行順序 -> 木構造で対応できそう
* <,<<,>,>>がある時の処理の流れが曖昧？->実験結果より、整形できる
* here_doc tmpfileの存在と削除の方法 -> tmpの作成等は特に調査せず。file deleteはunlinkで実装できた

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

#### redirect in
```c
$> < file cmd
$> cmd < file // file -> cmd
 1. fd = open(file, READ)
 2. dup2(fd, STDOUT)
 3. exec cmd
// check in playground/test_redirect.c

input[<,file,cmd] or [cmd,<,file]
analysisで<をチェック,redirect_in/out flgをつける && ファイル名を取得
file < or < file

< infile1 cmd < infile2             : cmd < infile2
< infile1 cmd < infile2 < infile3   : cmd < infile3
cmd < infile1 < infile2 < infile3   : cmd < infile3
いずれもfileがなければerror

1. cmd[i] == "<", cmd[i+1] == "file" を探す
2. redirect_in flg
3. filenameを記録
4. char **cmdsからredirect部分を削除する or command_head_idxを持っておく?
    -> command < "<"の時にexecveに渡されると, invalid opsionになりそう
	-> redirect部分を削除する
5. command実行時にfile_open, etcを実行する
    -> exec_redirect(cmds, file, flg);

```

```c
int main(void)
{
    // test cmd < infile
	char *cat[10] = {"cat", "-e", NULL};
	char *file = "infile";
	int	file_fd[2];

	file_fd[READ] = open(file, O_RDONLY);
	dup2(file_fd[READ], STDIN_FILENO);
	execvp(cat[0], cat);

	return (0);
}

//output
% cat infile 
this is testfile
hoge
foo

next EOF
% ./a.out
this is testfile$
hoge$
foo$
$
next EOF$
```
<br>

#### redirect out

```c
$> cmd > file
$> > file cmd
 1. fd = open(file, WRITE)
 2. dup2(fd, STDIN)
 3. exec cmd
// check in playground/test_redirect.c
```

```c
int main(void)
{
	// test cmd > outfile
	char *ls[10] = {"ls", "-l", NULL};
	char *file = "outfile";
	int	file_fd[2];

	file_fd[WRITE] = open(file, O_WRONLY);
	dup2(file_fd[WRITE], STDOUT_FILENO);
	execvp(ls[0], ls);

	return (0);
}

//output
% cat outfile
% ./a.out
% cat outfile
total 144
-rwxr-xr-x  1 akira  staff  33682 Jan  7 16:54 a.out
-rw-r--r--  1 akira  staff  10909 Jan  7 16:52 idea.md
-rw-r--r--  1 akira  staff     36 Jan  7 16:50 infile
-rw-r--r--  1 akira  staff      0 Jan  7 16:52 outfile
-rw-r--r--  1 akira  staff    391 Jan  4 18:32 test.h
-rw-r--r--  1 akira  staff    514 Jan  7 16:38 test_cd.c
-rw-r--r--  1 akira  staff   1352 Jan  7 16:54 test_exec.c
-rw-r--r--  1 akira  staff   2238 Jan  7 16:38 test_fork.c
-rw-r--r--  1 akira  staff   1140 Jan  7 11:33 test_tree.c
-rw-r--r--  1 akira  staff      0 Jan  5 14:44 testfile

```

#### 多段redirect
```c
int main(void)
{
	// test cmd > via1 > via2 > out
	char *ls[10] = {"ls", "-l", NULL};
	char *via1 = "via1";
	char *via2 = "via2";
	char *out = "out";
	int fd_via1, fd_via2, fd_out;

	fd_via1 = open(via1, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	fd_via2 = open(via2, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	fd_out = open(out, O_CREAT | O_WRONLY | O_TRUNC, 0666);

	dup2(fd_via1, STDOUT_FILENO);
	close(fd_via1);
	dup2(fd_via2, STDOUT_FILENO);
	close(fd_via2);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);

	execvp(ls[0], ls);

	return (0);
}
// output
% gcc test_exec.c 
% ./a.out
% cat via1
% cat via2
% cat out
total 176
-rwxr-xr-x  1 akira  staff  33682 Jan  7 18:41 a.out
-rw-r--r--  1 akira  staff  12557 Jan  7 17:48 idea.md
-rw-r--r--  1 akira  staff     36 Jan  7 16:50 infile
-rw-r--r--  1 akira  staff      8 Jan  7 17:23 infile2
-rw-r--r--  1 akira  staff      8 Jan  7 17:23 infile3
-rw-r--r--  1 akira  staff      0 Jan  7 18:41 out
-rw-r--r--  1 akira  staff    571 Jan  7 16:54 outfile
-rw-r--r--  1 akira  staff    391 Jan  4 18:32 test.h
-rw-r--r--  1 akira  staff    514 Jan  7 16:38 test_cd.c
-rw-r--r--  1 akira  staff   1815 Jan  7 18:41 test_exec.c
-rw-r--r--  1 akira  staff   2238 Jan  7 16:38 test_fork.c
-rw-r--r--  1 akira  staff   1140 Jan  7 11:33 test_tree.c
-rw-r--r--  1 akira  staff      0 Jan  5 14:44 testfile
-rw-r--r--  1 akira  staff      0 Jan  7 18:41 via1
-rw-r--r--  1 akira  staff      0 Jan  7 18:41 via2

```
```c
$> < infile cat -e test1 test2 test3
//test1$ (cat -e test1)
//test2$ (cat -e test2)
//test3$ (cat -e test3)

$> cat -e test1 test2 test3 < infile
//test1$ (cat -e test1)
//test2$ (cat -e test2)
//test3$ (cat -e test3)
//infileは無視される？
//redirectはコマンド行から消すからOK
cat redirect < 引数jec
```

```c
< infile = <infile
> outfile = >outfile

分割が必要
```

```c
// TODO: check multi case
<infile1 <infile2 <infile3 cmd //cmd <infile3
<infile1 <infile2 <infile3 cmd <infile4 //cmd < infile4
<infile cmd < //syntax error
<infile1<infile2

<infile cat >out //out: infile
<infile1 <infile2 cat > out //out:infile2
<infile1 <infile2 cat <infile3 > out //out:infile3


bash-3.2$ <test1 cat > out1 << end
> hogehoge
> end
bash-3.2$ cat out1
hogehoge
bash-3.2$
//これやばいな
//優先順が良くわからん


bash-3.2$ <test1 cat > out1 << end > out2
> hogehoge
> end
bash-3.2$ cat out1
bash-3.2$ cat out2
hogehoge
bash-3.2$
//<infile1 <infile2 <infile3 cmd <infile4 //cmd < infile4 と同様


bash-3.2$ <test1 cat > out1 << end > out2 < test2
> hoge
> hogehoge
> end
bash-3.2$ cat test1
test1
bash-3.2$ cat out1
bash-3.2$ cat out2
test2
bash-3.2$ cat test2
test2
bash-3.2$
// <infile1 <infile2 <infile3 cmd <infile4 //cmd < infile4　と同様


bash-3.2$ <test111 cat > out1 << end > out2 < test2
> aa
> aa
> end
bash: test111: No such file or directory
bash-3.2$ cat out1
out1
bash-3.2$ cat out2
out2
bash-3.2$ cat test
cat: test: No such file or directory
//存在しないtest111, out1, out2は初期状態のまま（初期化されない）
//here_docは最優先で実行されるみたい？
//&&, ||の時の挙動も要確認?


bash-3.2$ <test1 cat > out1 << end1 << end2 > out2 <<end3 > out
> hoge
> end3
> end2
> end
> end1
>
>
> hogehoge
> test
> end2
> fooooo
> test
> end3
bash-3.2$ cat out1
bash-3.2$ cat out2
bash-3.2$ cat out
fooooo
test
bash-3.2$
// here_docは前から実行される
// 実行に成功すると、outfileが初期化される
// 優先順位は、末端の << end3 > out が有効になる

bash-3.2$ <test1 cat > out1 << end1 << end2 ls > out2 <<end3 > out
> aa
> aa
> end1
> bb
> bb
> end2
> vv
> vv
> end3
cat: ls: No such file or directory
bash-3.2$ cat out1
bash-3.2$ cat out2
bash-3.2$ cat out
// 失敗しても初期化される？

bash-3.2$ <test111 cat > out1 << end1 << end2 ls > out2 <<end3 > out
> aaa
> end1
> avvv
> end2
> eeee
> end3
bash: test111: No such file or directory
bash-3.2$ echo out1
out1
bash-3.2$ echo out2
out2
bash-3.2$ echo out
out
bash-3.2$
//エラーによっては初期化されない
//まずfileをすべて開くのか？
// here_doc -> fileopen -> command

bash-3.2$ <test1 cat > out1 > out2 <test2
bash-3.2$ cat out2
test2
bash-3.2$

bash-3.2$ <in1 cat >out1 >out2 <in2
bash-3.2$ cat in1; cat in2; cat out1; cat out2;
in1
in2
in2
bash-3.2$ cat out2
in2
bash-3.2$

1. <in1 
2. cat >out1 >out2
3. <in2
2, 3が採用?

<in2 cat >out2に整形できる

<をすべて見て,最終段にあるものをstdinとする <in2
>をすべて見て,最終段にあるものにstdoutをつなげる >res
file openする
redirect除いたcommandを実行する


bash-3.2$ <in1 cat >out1 >out2 <in2 > res
bash-3.2$ cat res
in2
bash-3.2$ cat out2
bash-3.2$ cat out2
bash-3.2$


// コマンドが複数存在した場合？
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