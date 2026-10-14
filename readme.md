# ライブラリ一覧

公開先: https://github.com/Katu2ou/katu2ou-icpc-regional

関連ライブラリ: https://github.com/Katu2ou/Katu2ou-Cpro-Library

`template.cpp` の先頭に、両リポジトリのトップページURLを記載しています。
[AtCoder生成AI対策ルール](https://info.atcoder.jp/entry/llm-rules-ja)に従い、
事前作成したコードを使用するときは、その使用版をコンテスト開始前にWebで公開し、
コピーして提出するコードに参照URLを残してください。

## データ構造
- segtree
    - ACLからmax_left, min_rightを除いたもの
- lazy_segtree
    - ACLからmax_left, min_rightを除いたもの
- dynamic_segtree
- dsu
    - ACLに準拠
- trie
- binary trie
- cartesian tree
- convex hull trick
- wavelet matrix
- 2D segtree(疎なN^2(N~10^5)に対応)

## 文字列
- suffix array
    - ACLに準拠
- lcp array
    - ACLに準拠
- z algorithm
    - ACLに準拠
- rolling_hash
    - mod 2^{61}-1のやつ
- manacher

## 数学
- pow_mod, inv_mod, crt, floor_sum
    - ACLに準拠

## グラフ
- scc
    - ACLに準拠
- euler tour
- heavy-light decomposition

## 幾何
- geometry(long double)

## 線形代数
- linear equation
- linear equation(bitset)

## 畳み込み
- modint, convolution
    - ACLに準拠
    - modは998244353を想定 
    - modintは簡単にmodを変えられる 

## フロー
- maxflow, mincostflow 
    - ACLに準拠, slopeを使わないなら30-40行程度

## その他欲しいもの
- lowlink
- 区間をsetで管理するやつ

