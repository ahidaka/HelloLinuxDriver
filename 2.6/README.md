# Hello Linux Driver （Kernel 2.6 用）

Linux ドライバー開発のための小さなサンプルとヒントです。

## hello サブディレクトリ

単純なローダブルモジュールのドライバーと、デバッグ用パラメーター受け渡しのサンプルです。

### hello.ko -- 単純ドライバー

起動時に渡された *debug* パラメーターの値を *printk* で表示します。

実行例
```sh
# insmod hello.ko debug=1
# dmesg
```

### hello_m.ko (hello_mult.c, hello_extern.c) -- 複数ソース・ファイルのサンプル
モジュールパラメーター */sys/module/hello_m/parameters/sw* の値の読み書きで動作します。

実行例
```sh
# insmod hello_m.ko
# echo 1 > /sys/module/hello_m/parameters/sw
# cat /sys/module/hello_m/parameters/sw
# dmesg
```

### params.ko -- モジュールパラメーター・サンプル
次のパラメーターの利用事例を示します。
* sw -- bool型 パラメーター

* debug -- int型 パラメーター、Read/Write時に個別ルーチン実行

* sheet -- string型 パラメーター

* line -- string型 パラメーター、Read/Write時に個別ルーチン実行

* tabsize -- int型 パラメーター、table 配列の有効要素数の受け渡し用

* table --  int 配列型 パラメーター

実行例
```sh
# insmod params.ko
# echo 1 > /sys/module/params/parameters/sw
# cat /sys/module/params/parameters/sw
# echo null > /sys/module/params/parameters/sw
# cat /sys/module/params/parameters/sw
# echo 1 > /sys/module/params/parameters/debug
# cat /sys/module/params/parameters/debug
# echo "free string" > /sys/module/params/parameters/sheet
# cat /sys/module/params/parameters/sheet
# echo text > /sys/module/params/parameters/line
# cat /sys/module/params/parameters/line
# cat /sys/module/params/parameters/tabsize
# cat /sys/module/params/parameters/table
# echo 0,1,2,3 > /sys/module/params/parameters/table
# cat /sys/module/params/parameters/tabsize
# dmesg
```

## schedule サブディレクトリ

カーネルスレッドと各種の遅延実行処理のサンプルです。詳細はソースコードを参照してください。

### period.ko -- schedule_timeout 遅延実行サンプル
実行例
```sh
# insmod period.ko
# echo 500 > /sys/module/period/parameters/sleep
# echo 5 > /sys/module/period/parameters/count
# echo 1 > /sys/module/period/parameters/sw
# dmesg
```

### kthread.ko -- kthread (Kernel Thread) 実行サンプル
実行例
```sh
# insmod kthread.ko
# echo 500 > /sys/module/kthread/parameters/sleep
# echo 5 > /sys/module/kthread/parameters/count
# echo 1 > /sys/module/kthread/parameters/sw
# dmesg
```

### tasklet.ko -- tasklet 実行サンプル
実行例
```sh
# insmod tasklet.ko
# echo 100000 > /sys/module/tasklet/parameters/sleep
# echo 1 > /sys/module/tasklet/parameters/sw
# dmesg
```

### workq.ko -- 汎用 Workqueue サンプル
実行例
```sh
# insmod workq.ko
# echo 1 > /sys/module/workq/parameters/sw
# cat /sys/module/workq/parameters/sw
# dmesg
```

### workq2.ko -- カスタム Workqueue サンプル
実行例
```sh
# insmod workq2.ko
# echo 1000 > /sys/module/period/period/sleep
# echo 5 > /sys/module/period/period/count
# echo 1 > /sys/module/period/period/sw
# dmesg
```
