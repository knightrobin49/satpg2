# satpg

## はじめに

satpg は SAT ソルバを用いたテスト生成プログラムです．
ビルドに関しては BUILDING.md ファイルを参照してください．

現時点では Tcl/Tk とバインディングした atpg_tclsh のみが
提供されています．将来的には Python/Cython 拡張を行う予定です．

## atpg_tclsh

satpg の機能を Tcl/Tk の拡張コマンドとして実装したプログラムです．
通常の tclsh に加えて以下のコマンドが追加されています．

- ::atpg::read_blif

  blif 形式のファイル読み込み

- ::atpg::read_iscas89

  iscas89(.bench) 形式のファイル読み込み

- ::atpg::rtpg

  ランダムパタンを用いた故障シミュレーション

- ::atpg::dtpg

  SAT を用いたテストパタン生成

- ::atpg::minpat

  SAT を用いたテストパタン生成(テストパタン圧縮機能付き)

- ::atpg::print_stats

  状態表示

- ::atpg::print_faults

  故障の状態表示

- ::atpg::print_pat

  パタン出力

- ::atpg::print_pat_stats

  パタンの状態表示

個々のコマンドについては，

```tcl
$ <コマンド> -h
```

でヘルプが出力されます．