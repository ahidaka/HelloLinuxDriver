# Hello Linux Driver （Kernel 6.5 用）

Linux ドライバー開発入門用 sysfs パラメータ機能とスレッドスケジュール のサンプル

- sysfs パラメータ受け渡し機能は、専用UI（ユーザーインターフェース）を持たない Linux ドライバーにおいて、実質的なUIであり、ほとんどの Linux ドライバーが持つ基本的な機能です。
- スレッドスケジュール機能は、カーネルモジュールを自由に使いこなすために重要なLinuxカーネルの基本機能です。

## hello ディレクトリ

単純なローダブルモジュールのドライバーと、デバッグ用パラメーター受け渡しのサンプル
- helllo.ko
- hello_m.ko
- params.ko

### hello.ko - ハロードライバー

*debug* パラメータを持つ、最も単純なローダブルモジュール・ドライバー。

#### カーネルモジュールの基本操作確認

起動時または *sysfs* パラメータで渡された *debug* パラメーターの値を保持して、*printk* でカーネルログに表示します。
さらに *sysfs* のパラメータ管理と *dmesg*, *syslog* ログ表示機能を確認します。

#### 演習： ロードとパラメータ指定, sysfs

```sh
# ls /sys/module/
# modinfo hello.ko
# insmod hello.ko debug=1
# dmesg
# ls /sys/module/
```

#### 解説

- */sys/module/* の *sysfs* に *hello* モジュールのツリーが無いことを確認
- *modinfo* コマンドで、モジュール情報を確認
- *debug=1* オプション付きで *hello.ko* モジュールをロード
- *dmesg* コマンドでカーネルメッセージ出力を確認
- */sys/module/* の *sysfs* に *hello* モジュールのツリーが作成されたことを確認

*/sys* 以下のディレクトリツリーは **sysfs** と呼び、ドライバーとローダブルモジュールの管理とインターフェースを提供します。

#### 演習： sysfs パラメータ指定とアンロード

```sh
# cat /sys/module/hello/parameters/debug
# echo 2 > /sys/module/hello/parameters/debug
# cat /sys/module/hello/parameters/debug
# rmmod hello
# ls /sys/module/
# dmesg
# less /var/log/syslog
```

#### 解説

- */sys/module/hello* エントリーの作成と debug パラメータ値を確認
- コマンド操作で *debug* パラメータ値 "2" を書き込み
- 書き込んだ の */sys/module/hello* の *debug* パラメータ値の確証
- *helllo* モジュールをアンロード
- */sys/module/* に *hello* モジュールのツリーの削除を確認
- *dmesg* コマンドでカーネルメッセージ出力を確認
- */var/log/syslog* カーネルログメッセージを確認

<br/>

### hello_m.ko (hello_mult.c, hello_extern.c) -- 複数ソース・ファイルドライバー

複数ソースコードで構成する *hello_m.ko* モジュールのパラメーター */sys/module/hello_m/parameters/sw* の値の読み書きを実行するドライバー

#### 演習

```sh
# modinfo hello_m.ko
# insmod hello_m.ko
# cat /sys/module/hello_m/parameters/sw
# echo 1 > /sys/module/hello_m/parameters/sw
# dmesg
# less /var/log/syslog
```

#### 解説

- *modinfo* コマンドで、モジュール情報を確認
- オプション無しで *helllo_m.ko* モジュールをロード
- */sys/module/hello_m* エントリーと *sw* パラメータ値を確認
- */sys/module/hello_m* の *sw* パラメータに 1 を書き込み
- *dmesg* コマンドでカーネルメッセージ出力を確認
- */var/log/syslog* カーネルログメッセージを確認

<br/>

### params.ko -- モジュールパラメーター・ドライバー

次の様々な型のパラメーターと、コールバック(cb)パラメータのサンプル・ドライバー

- *sw* -- bool型 パラメーター
- *debug* -- int型 パラメーター、Read/Write時に個別コールバックルーチン実行
- *sheet* -- string型 パラメーター
- *line* -- string型 パラメーター、Read/Write時に個別コールバックルーチン実行
- *tabsize* -- int型 パラメーター、table 配列の有効要素数の受け渡し用
- *table* --  int 配列型 パラメーター

#### 演習

```sh
# modinfo params.ko
# insmod params.ko
# more /sys/module/params/parameters/*
# echo 1 > /sys/module/params/parameters/sw
# cat /sys/module/params/parameters/sw
# echo null > /sys/module/params/parameters/sw
# cat /sys/module/params/parameters/sw
# echo 5 > /sys/module/params/parameters/debug
# cat /sys/module/params/parameters/debug
# echo "free string" > /sys/module/params/parameters/sheet
# cat /sys/module/params/parameters/sheet
# echo text > /sys/module/params/parameters/line
# cat /sys/module/params/parameters/line
# cat /sys/module/params/parameters/table
# echo 0,1,2,3 > /sys/module/params/parameters/table
# cat /sys/module/params/parameters/tabsize
# dmesg
# less /var/log/syslog
```

#### 解説

- *modinfo* コマンドで、モジュール情報を確認
- オプション無しで *params.ko* モジュールをロード
- 各パラメータ初期値を表示して確認
- *sw* パラメータ（bool型）に 1 を書き込み
- 書き込んだ *sw* パラメータ値を確認
- *sw* パラメータ値に *null* を書き込み
- 書き込んだ *sw* パラメータ値を確認
- *debug* パラメータ（int型）に 5 を書き込み
- 書き込んだ *debug* パラメータ値を確認
- *sheet* パラメータ（string型）に "free string" を書き込み
- 書き込んだ *sheet* パラメータ値を確認
- *line* パラメータ（string型）に "free string" を書き込み
- 書き込んだ *line* パラメータ値を確認
- *tabsize*  パラメータ値を確認
- *table*  パラメータに 0,1,2,3 を書き込み
- *tabsize*  パラメータ値を確認
- *dmesg* コマンドでカーネルメッセージ出力を確認
- */var/log/syslog* カーネルログメッセージを確認

個別コールバックルーチンを呼び出しているので、ソースコード params.c の内容とデバッグメッセージで動作を確認します。

<br/>

## schedule ディレクトリ

各種カーネルスレッドと遅延実行処理サンプル

- period.ko
- kthread.ko
- tasklet.ko
- workq.ko
- workq2.ko

### period.ko -- schedule_timeout 遅延実行サンプル

ユーザーアプリケーション(echo)からの呼び出しで、カーネルモードでブロック（同期）実行する単純なドライバーモジュールのサンプル。内部でパラメータ時間ブロックスリープするため、終了するまで制御が戻りません。
*sleep* パラメータは *jiffies* 値（通常ms）、*count* 回数分スリープします。

#### 演習

```sh
# modinfo period.ko 
# insmod period.ko
# more /sys/module/period/parameters/*
# echo 500 > /sys/module/period/parameters/sleep
# echo 5 > /sys/module/period/parameters/count
# echo 1 > /sys/module/period/parameters/sw
# dmesg
```

#### 解説

- *modinfo* コマンドで、モジュール情報を確認
- オプション無しで *period.ko* モジュールをロード
- 各パラメータ初期値を表示して確認
- *sleep* パラメータに 500 を書き込み
- *count* パラメータに 5 を書き込み
- *sw* パラメータに 1 を書き込んで実行開始
- *dmesg* コマンドでカーネルメッセージ出力を確認

<br/>

### kthread.ko -- kthread (Kernel Thread) 実行サンプル

カーネルスレッドの基本動作である、kthread関数（kthread_run）のサンプルです。
内部で schedule_timeout 関数呼び出しで、パラメータ時間スリープしますが非同期（ノンブロック）動作のため、制御がすぐに戻ります。
*sleep* パラメータは *jiffies* 値（通常ms）、*count* 回数分スリープします。

#### 演習

```sh
# modinfo kthread.ko 
# insmod kthread.ko
# more /sys/module/kthread/parameters/*
# echo 500 > /sys/module/kthread/parameters/sleep
# echo 5 > /sys/module/kthread/parameters/count
# echo 1 > /sys/module/kthread/parameters/sw
# dmesg
```
#### 解説

- *modinfo* コマンドで、モジュール情報を確認
- オプション無しで *kthread.ko* モジュールをロード
- 各パラメータ初期値を表示して確認
- *sleep* パラメータに 500 を書き込み
- *count* パラメータに 5 を書き込み
- *sw* パラメータに 1 を書き込んで実行開始
- *dmesg* コマンドでカーネルメッセージ出力を確認

<br/>

### tasklet.ko -- tasklet 実行サンプル

taskletは割り込みハンドラからソフトウェア割り込みで遅延実行されるカーネルタスクです。
割り込みで動作するため、処理内容には制約があります。
このサンプルは実際には割り込みハンドラから実行しないため、書き方のだけのサンプルです。
*loop* パラメータは tasklet 1回処理毎の実行数、*count* は実行回数です。

#### 演習

```sh
# modinfo tasklet.ko
# insmod tasklet.ko
# more /sys/module/tasklet/parameters/*
# echo 20 > /sys/module/tasklet/parameters/count
# echo 1 > /sys/module/tasklet/parameters/sw
# dmesg
```
#### 解説

- *modinfo* コマンドで、モジュール情報を確認
- オプション無しで *tasklet.ko* モジュールをロード
- 各パラメータ初期値を表示して確認
- *count* パラメータに 20 を書き込んで実行回数指定
- *sw* パラメータに 1 を書き込んで実行開始
- *dmesg* コマンドでカーネルメッセージ出力を確認

<br/>

### workq.ko -- 汎用 Workqueue サンプル

workq は、割り込みハンドラから遅延実行されるカーネルタスクです。
このサンプルではカーネルが用意した汎用workqueueを使用します。

#### 演習

```sh
# modinfo workq.ko
# insmod workq.ko
# more /sys/module/workq/parameters/*
# echo 1 > /sys/module/workq/parameters/sw
# cat /sys/module/workq/parameters/sw
# dmesg
```

#### 解説

- *modinfo* コマンドで、モジュール情報を確認
- オプション無しで *workq.ko* モジュールをロード
- 各パラメータ初期値を表示して確認
- *sw* パラメータに 1 を書き込んで実行開始
- *dmesg* コマンドでカーネルメッセージ出力を確認

<br/>

### workq2.ko -- カスタム Workqueue サンプル

workq は、割り込みハンドラから遅延実行されるカーネルタスクです。
このサンプルでは個別に用意した専用のworkqueueを使用します。

#### 演習

```sh
# modinfo workq2.ko
# insmod workq2.ko
# more /sys/module/workq2/parameters/*
# echo 1 > /sys/module/workq2/period/sw
# cat /sys/module/workq2/parameters/sw
# dmesg
```

#### 解説

- *modinfo* コマンドで、モジュール情報を確認
- オプション無しで *workq2.ko* モジュールをロード
- 各パラメータ初期値を表示して確認
- *sw* パラメータに 1 を書き込んで実行開始
- *dmesg* コマンドでカーネルメッセージ出力を確認

<br/>
