# 関数別オンライン検証結果

更新: 2026-09-21T16:13:54+09:00

32 ファイルに明示的に定義された名前付き関数・コンストラクタ・オーバーロード 207 件を列挙しました。内部の再帰用関数や private な補助関数は、それらを実際に呼び出す公開 API の提出に対応づけています。無名ラムダは親関数の一部として扱い、関数ではないマクロ・型エイリアスは件数に含めていません。

AC確認あり: **167 件** | オンライン結果待ち: **30 件** | TLE: **8 件** | WA: **2 件**

**「AC確認あり」は、リンク先の少なくとも一つの提出で当該関数を実行し、提出全体が AC だったことを表します。**すべての型・引数・実行経路を数学的に証明するものではありません。未提出の検証も表中に残してあり、AC と区別しています。

## 今回の追加提出

| 検証コード | 問題 | オンライン結果 |
| --- | --- | --- |
| [lc/wavelet_all](full/submissions/wavelet_all.cc) | [range_kth_smallest](https://judge.yosupo.jp/problem/range_kth_smallest) | [AC](https://judge.yosupo.jp/submission/404441) |
| [lc/wavelet_frequency](full/submissions/wavelet_frequency.cc) | [static_range_frequency](https://judge.yosupo.jp/problem/static_range_frequency) | [AC](https://judge.yosupo.jp/submission/404442) |
| [lc/segtree_search](full/submissions/segtree_search.cc) | [predecessor_problem](https://judge.yosupo.jp/problem/predecessor_problem) | [AC](https://judge.yosupo.jp/submission/404443) |
| [lc/lazy_segtree_search](full/submissions/lazy_segtree_search.cc) | [predecessor_problem](https://judge.yosupo.jp/problem/predecessor_problem) | [AC](https://judge.yosupo.jp/submission/404445) |
| [lc/lazy_segtree_range_search](full/submissions/lazy_segtree_range_search.cc) | [range_affine_range_sum](https://judge.yosupo.jp/problem/range_affine_range_sum) | [AC](https://judge.yosupo.jp/submission/404446) |
| [lc/binarytrie_max](full/submissions/binarytrie_max.cc) | [set_xor_min](https://judge.yosupo.jp/problem/set_xor_min) | [AC](https://judge.yosupo.jp/submission/404447) |
| [lc/bit_lower_bound](full/submissions/bit_lower_bound.cc) | [predecessor_problem](https://judge.yosupo.jp/problem/predecessor_problem) | [AC](https://judge.yosupo.jp/submission/404448) |
| [lc/dsu_size](full/submissions/dsu_size.cc) | [unionfind](https://judge.yosupo.jp/problem/unionfind) | [AC](https://judge.yosupo.jp/submission/404449) |
| [lc/suffix_array_integer](full/submissions/suffix_array_integer.cc) | [suffixarray](https://judge.yosupo.jp/problem/suffixarray) | [AC](https://judge.yosupo.jp/submission/404450) |
| [lc/bit_range_add](full/submissions/bit_range_add.cc) | [static_range_sum](https://judge.yosupo.jp/problem/static_range_sum) | [AC](https://judge.yosupo.jp/submission/404453) |
| [lc/hld_jump](full/submissions/hld_jump.cc) | [jump_on_tree](https://judge.yosupo.jp/problem/jump_on_tree) | [AC](https://judge.yosupo.jp/submission/404454) |
| [lc/maxflow_helpers](full/submissions/maxflow_helpers.cc) | [bipartitematching](https://judge.yosupo.jp/problem/bipartitematching) | [AC](https://judge.yosupo.jp/submission/404457) |
| [lc/bmbm_wrapper](full/submissions/bmbm_wrapper.cc) | [find_linear_recurrence](https://judge.yosupo.jp/problem/find_linear_recurrence) | [AC](https://judge.yosupo.jp/submission/404459) |
| [lc/dynamic_all_prod](full/submissions/dynamic_all_prod.cc) | [point_set_range_composite](https://judge.yosupo.jp/problem/point_set_range_composite) | [AC](https://judge.yosupo.jp/submission/404460) |
| [lc/mint_all](full/submissions/mint_all.cc) | [convolution_mod](https://judge.yosupo.jp/problem/convolution_mod) | [AC](https://judge.yosupo.jp/submission/404461) |
| [lc/cartesian_all](full/submissions/cartesian_all.cc) | [cartesian_tree](https://judge.yosupo.jp/problem/cartesian_tree) | [AC](https://judge.yosupo.jp/submission/404470) |
| [lc/mo_directional](full/submissions/mo_directional.cc) | [static_range_count_distinct](https://judge.yosupo.jp/problem/static_range_count_distinct) | [AC](https://judge.yosupo.jp/submission/404471) |
| [lc/rerooting_nonzero_root](full/submissions/rerooting_nonzero_root.cc) | [tree_path_composite_sum](https://judge.yosupo.jp/problem/tree_path_composite_sum) | [AC](https://judge.yosupo.jp/submission/404472) |
| [lc/maxflow_nonzero_change](full/submissions/maxflow_nonzero_change.cc) | [bipartitematching](https://judge.yosupo.jp/problem/bipartitematching) | [AC](https://judge.yosupo.jp/submission/404473) |
| [lc/template_entry](full/submissions/template_entry.cc) | [aplusb](https://judge.yosupo.jp/problem/aplusb) | [WA](https://judge.yosupo.jp/submission/404462) |
| [lc/mincost_slope_assignment](full/submissions/mincost_slope_assignment.cc) | [assignment](https://judge.yosupo.jp/problem/assignment) | [TLE](https://judge.yosupo.jp/submission/404478) |
| [geometry/geo_hull](full_geometry/submissions/geo_hull.cc) | [static_convex_hull](https://judge.yosupo.jp/problem/static_convex_hull) | [AC](https://judge.yosupo.jp/submission/404465) |
| [aoj/geo_projection](aoj/submissions/geo_projection.cc) | [CGL_1_A](https://onlinejudge.u-aizu.ac.jp/problems/CGL_1_A) | 未提出（AOJ認証待ち） |
| [aoj/geo_reflection](aoj/submissions/geo_reflection.cc) | [CGL_1_B](https://onlinejudge.u-aizu.ac.jp/problems/CGL_1_B) | 未提出（AOJ認証待ち） |
| [aoj/geo_orientation](aoj/submissions/geo_orientation.cc) | [CGL_1_C](https://onlinejudge.u-aizu.ac.jp/problems/CGL_1_C) | 未提出（AOJ認証待ち） |
| [aoj/geo_parallel](aoj/submissions/geo_parallel.cc) | [CGL_2_A](https://onlinejudge.u-aizu.ac.jp/problems/CGL_2_A) | 未提出（AOJ認証待ち） |
| [aoj/geo_segments_intersect](aoj/submissions/geo_segments_intersect.cc) | [CGL_2_B](https://onlinejudge.u-aizu.ac.jp/problems/CGL_2_B) | 未提出（AOJ認証待ち） |
| [aoj/geo_line_cross](aoj/submissions/geo_line_cross.cc) | [CGL_2_C](https://onlinejudge.u-aizu.ac.jp/problems/CGL_2_C) | 未提出（AOJ認証待ち） |
| [aoj/geo_distance](aoj/submissions/geo_distance.cc) | [CGL_2_D](https://onlinejudge.u-aizu.ac.jp/problems/CGL_2_D) | 未提出（AOJ認証待ち） |
| [aoj/geo_area](aoj/submissions/geo_area.cc) | [CGL_3_A](https://onlinejudge.u-aizu.ac.jp/problems/CGL_3_A) | 未提出（AOJ認証待ち） |
| [aoj/geo_convex](aoj/submissions/geo_convex.cc) | [CGL_3_B](https://onlinejudge.u-aizu.ac.jp/problems/CGL_3_B) | 未提出（AOJ認証待ち） |
| [aoj/geo_contained](aoj/submissions/geo_contained.cc) | [CGL_3_C](https://onlinejudge.u-aizu.ac.jp/problems/CGL_3_C) | 未提出（AOJ認証待ち） |
| [aoj/geo_hull_boundary](aoj/submissions/geo_hull_boundary.cc) | [CGL_4_A](https://onlinejudge.u-aizu.ac.jp/problems/CGL_4_A) | 未提出（AOJ認証待ち） |
| [aoj/geo_circle_relation](aoj/submissions/geo_circle_relation.cc) | [CGL_7_A](https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_A) | 未提出（AOJ認証待ち） |
| [aoj/geo_incircle](aoj/submissions/geo_incircle.cc) | [CGL_7_B](https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_B) | 未提出（AOJ認証待ち） |
| [aoj/geo_circumcircle](aoj/submissions/geo_circumcircle.cc) | [CGL_7_C](https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_C) | 未提出（AOJ認証待ち） |
| [aoj/geo_circle_line](aoj/submissions/geo_circle_line.cc) | [CGL_7_D](https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_D) | 未提出（AOJ認証待ち） |
| [aoj/geo_circle_cross](aoj/submissions/geo_circle_cross.cc) | [CGL_7_E](https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_E) | 未提出（AOJ認証待ち） |
| [aoj/geo_tangent_point](aoj/submissions/geo_tangent_point.cc) | [CGL_7_F](https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_F) | 未提出（AOJ認証待ち） |
| [aoj/geo_tangents](aoj/submissions/geo_tangents.cc) | [CGL_7_G](https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_G) | 未提出（AOJ認証待ち） |
| [aoj/geo_angles](aoj/submissions/geo_angles.cc) | [ITP1_10_B](https://onlinejudge.u-aizu.ac.jp/problems/ITP1_10_B) | 未提出（AOJ認証待ち） |
| [aoj/math_power](aoj/submissions/math_power.cc) | [NTL_1_B](https://onlinejudge.u-aizu.ac.jp/problems/NTL_1_B) | 未提出（AOJ認証待ち） |
| [aoj/math_bezout](aoj/submissions/math_bezout.cc) | [NTL_1_E](https://onlinejudge.u-aizu.ac.jp/problems/NTL_1_E) | 未提出（AOJ認証待ち） |
| [aoj/math_crt](aoj/submissions/math_crt.cc) | [NTL_1_E](https://onlinejudge.u-aizu.ac.jp/problems/NTL_1_E) | 未提出（AOJ認証待ち） |
| [aoj/mincost_slope](aoj/submissions/mincost_slope.cc) | [GRL_6_B](https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B) | 未提出（AOJ認証待ち） |
| [aoj/mincost_slope_unlimited](aoj/submissions/mincost_slope_unlimited.cc) | [GRL_6_B](https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B) | 未提出（AOJ認証待ち） |

## 関数の一覧

| ファイル | 関数・オーバーロード | 確認状態 | 提出の証跡 |
| --- | --- | --- | --- |
| [binary_indexed_tree.cpp](../binary_indexed_tree.cpp) | `BIT()` | AC確認あり | [lc/bit_lower_bound: AC](https://judge.yosupo.jp/submission/404448) |
| [binary_indexed_tree.cpp](../binary_indexed_tree.cpp) | `BIT(n)` | AC確認あり | [previous/binary_indexed_tree: AC](https://judge.yosupo.jp/submission/404395) |
| [binary_indexed_tree.cpp](../binary_indexed_tree.cpp) | `add(i,x)` | AC確認あり | [previous/binary_indexed_tree: AC](https://judge.yosupo.jp/submission/404395) |
| [binary_indexed_tree.cpp](../binary_indexed_tree.cpp) | `add(l,r,x)` | AC確認あり | [lc/bit_range_add: AC](https://judge.yosupo.jp/submission/404453) |
| [binary_indexed_tree.cpp](../binary_indexed_tree.cpp) | `init` | AC確認あり | [lc/bit_lower_bound: AC](https://judge.yosupo.jp/submission/404448) |
| [binary_indexed_tree.cpp](../binary_indexed_tree.cpp) | `lower_bound` | AC確認あり | [lc/bit_lower_bound: AC](https://judge.yosupo.jp/submission/404448) |
| [binary_indexed_tree.cpp](../binary_indexed_tree.cpp) | `query(i)` | AC確認あり | [previous/binary_indexed_tree: AC](https://judge.yosupo.jp/submission/404395) |
| [binary_indexed_tree.cpp](../binary_indexed_tree.cpp) | `query(l,r)` | AC確認あり | [previous/binary_indexed_tree: AC](https://judge.yosupo.jp/submission/404395) |
| [binarytrie.cpp](../binarytrie.cpp) | `add` | AC確認あり | [previous/binarytrie: AC](https://judge.yosupo.jp/submission/404396) |
| [binarytrie.cpp](../binarytrie.cpp) | `erase` | AC確認あり | [previous/binarytrie: AC](https://judge.yosupo.jp/submission/404396) |
| [binarytrie.cpp](../binarytrie.cpp) | `insert` | AC確認あり | [previous/binarytrie: AC](https://judge.yosupo.jp/submission/404396) |
| [binarytrie.cpp](../binarytrie.cpp) | `max_xor` | AC確認あり | [lc/binarytrie_max: AC](https://judge.yosupo.jp/submission/404447) |
| [binarytrie.cpp](../binarytrie.cpp) | `min_xor` | AC確認あり | [previous/binarytrie: AC](https://judge.yosupo.jp/submission/404396) |
| [bmbm.cpp](../bmbm.cpp) | `BMBM` | AC確認あり | [lc/bmbm_wrapper: AC](https://judge.yosupo.jp/submission/404459) |
| [bmbm.cpp](../bmbm.cpp) | `BerlekampMassey` | AC確認あり | [previous/bmbm_recurrence: AC](https://judge.yosupo.jp/submission/404397) |
| [bmbm.cpp](../bmbm.cpp) | `BostanMori` | AC確認あり | [previous/bmbm_kth: AC](https://judge.yosupo.jp/submission/404398) |
| [cartesiantree.cpp](../cartesiantree.cpp) | `CartesianTree()` | AC確認あり | [lc/cartesian_all: AC](https://judge.yosupo.jp/submission/404470) |
| [cartesiantree.cpp](../cartesiantree.cpp) | `CartesianTree(vector)` | AC確認あり | [lc/cartesian_all: AC](https://judge.yosupo.jp/submission/404470) |
| [convex_hull_trick.cpp](../convex_hull_trick.cpp) | `L::f` | AC確認あり | [previous/convex_hull_trick: AC](https://judge.yosupo.jp/submission/404400) |
| [convex_hull_trick.cpp](../convex_hull_trick.cpp) | `add` | AC確認あり | [previous/convex_hull_trick: AC](https://judge.yosupo.jp/submission/404400) |
| [convex_hull_trick.cpp](../convex_hull_trick.cpp) | `convex_hull_trick` | AC確認あり | [previous/convex_hull_trick: AC](https://judge.yosupo.jp/submission/404400) |
| [convex_hull_trick.cpp](../convex_hull_trick.cpp) | `get` | AC確認あり | [previous/convex_hull_trick: AC](https://judge.yosupo.jp/submission/404400) |
| [convex_hull_trick.cpp](../convex_hull_trick.cpp) | `insert` | AC確認あり | [previous/convex_hull_trick: AC](https://judge.yosupo.jp/submission/404400) |
| [convex_hull_trick.cpp](../convex_hull_trick.cpp) | `qry` | AC確認あり | [previous/convex_hull_trick: AC](https://judge.yosupo.jp/submission/404400) |
| [dsu.cpp](../dsu.cpp) | `dsu` | AC確認あり | [previous/dsu: AC](https://judge.yosupo.jp/submission/404401) |
| [dsu.cpp](../dsu.cpp) | `leader` | AC確認あり | [previous/dsu: AC](https://judge.yosupo.jp/submission/404401) |
| [dsu.cpp](../dsu.cpp) | `merge` | AC確認あり | [previous/dsu: AC](https://judge.yosupo.jp/submission/404401) |
| [dsu.cpp](../dsu.cpp) | `same` | AC確認あり | [previous/dsu: AC](https://judge.yosupo.jp/submission/404401) |
| [dsu.cpp](../dsu.cpp) | `size` | AC確認あり | [lc/dsu_size: AC](https://judge.yosupo.jp/submission/404449) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `all_prod` | AC確認あり | [lc/dynamic_all_prod: AC](https://judge.yosupo.jp/submission/404460) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `dynamic_segtree()` | AC確認あり | [lc/dynamic_all_prod: AC](https://judge.yosupo.jp/submission/404460) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `dynamic_segtree(n)` | AC確認あり | [previous/dynamic_segtree: AC](https://judge.yosupo.jp/submission/404402)<br>[previous/dynamic_segtree_composite: AC](https://judge.yosupo.jp/submission/404437) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `get(node,a,b,p)` | AC確認あり | [previous/dynamic_segtree: AC](https://judge.yosupo.jp/submission/404402)<br>[previous/dynamic_segtree_composite: AC](https://judge.yosupo.jp/submission/404437) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `get(p)` | AC確認あり | [previous/dynamic_segtree: AC](https://judge.yosupo.jp/submission/404402)<br>[previous/dynamic_segtree_composite: AC](https://judge.yosupo.jp/submission/404437) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `node::node` | AC確認あり | [previous/dynamic_segtree: AC](https://judge.yosupo.jp/submission/404402)<br>[previous/dynamic_segtree_composite: AC](https://judge.yosupo.jp/submission/404437) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `node::update` | AC確認あり | [previous/dynamic_segtree: AC](https://judge.yosupo.jp/submission/404402)<br>[previous/dynamic_segtree_composite: AC](https://judge.yosupo.jp/submission/404437) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `prod(l,r)` | AC確認あり | [previous/dynamic_segtree: AC](https://judge.yosupo.jp/submission/404402)<br>[previous/dynamic_segtree_composite: AC](https://judge.yosupo.jp/submission/404437) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `prod(node,a,b,l,r)` | AC確認あり | [previous/dynamic_segtree: AC](https://judge.yosupo.jp/submission/404402)<br>[previous/dynamic_segtree_composite: AC](https://judge.yosupo.jp/submission/404437) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `set(node,a,b,p,x)` | AC確認あり | [previous/dynamic_segtree: AC](https://judge.yosupo.jp/submission/404402)<br>[previous/dynamic_segtree_composite: AC](https://judge.yosupo.jp/submission/404437) |
| [dynamic_segtree.cpp](../dynamic_segtree.cpp) | `set(p,x)` | AC確認あり | [previous/dynamic_segtree: AC](https://judge.yosupo.jp/submission/404402)<br>[previous/dynamic_segtree_composite: AC](https://judge.yosupo.jp/submission/404437) |
| [eulertour.cpp](../eulertour.cpp) | `EulerTour` | AC確認あり | [previous/eulertour: AC](https://judge.yosupo.jp/submission/404404) |
| [eulertour.cpp](../eulertour.cpp) | `dfs` | AC確認あり | [previous/eulertour: AC](https://judge.yosupo.jp/submission/404404) |
| [geometry.cpp](../geometry.cpp) | `Circle()` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `Circle(Point,r)` | オンライン結果待ち | [aoj/geo_circle_relation: 未提出](aoj/submissions/geo_circle_relation.cc) |
| [geometry.cpp](../geometry.cpp) | `ConvexHull` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465)<br>[aoj/geo_hull_boundary: 未提出](aoj/submissions/geo_hull_boundary.cc) |
| [geometry.cpp](../geometry.cpp) | `Line()` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `Line(A,B,C)` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `Line(Point,Point)` | オンライン結果待ち | [aoj/geo_projection: 未提出](aoj/submissions/geo_projection.cc) |
| [geometry.cpp](../geometry.cpp) | `PolygonArea` | オンライン結果待ち | [aoj/geo_area: 未提出](aoj/submissions/geo_area.cc) |
| [geometry.cpp](../geometry.cpp) | `SignedPolygonArea` | オンライン結果待ち | [aoj/geo_area: 未提出](aoj/submissions/geo_area.cc) |
| [geometry.cpp](../geometry.cpp) | `ccw` | オンライン結果待ち | [aoj/geo_orientation: 未提出](aoj/submissions/geo_orientation.cc) |
| [geometry.cpp](../geometry.cpp) | `circumCircle` | オンライン結果待ち | [aoj/geo_circumcircle: 未提出](aoj/submissions/geo_circumcircle.cc) |
| [geometry.cpp](../geometry.cpp) | `cross` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `crossPoint(Circle,Circle)` | オンライン結果待ち | [aoj/geo_circle_cross: 未提出](aoj/submissions/geo_circle_cross.cc) |
| [geometry.cpp](../geometry.cpp) | `crossPoint(Circle,Line)` | オンライン結果待ち | [aoj/geo_circle_line: 未提出](aoj/submissions/geo_circle_line.cc) |
| [geometry.cpp](../geometry.cpp) | `crossPoint(Line,Line)` | オンライン結果待ち | [aoj/geo_line_cross: 未提出](aoj/submissions/geo_line_cross.cc) |
| [geometry.cpp](../geometry.cpp) | `degreeToRadian` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `distanceBetweenLineAndPoint` | オンライン結果待ち | [aoj/geo_incircle: 未提出](aoj/submissions/geo_incircle.cc) |
| [geometry.cpp](../geometry.cpp) | `distanceBetweenSegmentAndPoint` | オンライン結果待ち | [aoj/geo_distance: 未提出](aoj/submissions/geo_distance.cc) |
| [geometry.cpp](../geometry.cpp) | `distanceBetweenSegments` | オンライン結果待ち | [aoj/geo_distance: 未提出](aoj/submissions/geo_distance.cc) |
| [geometry.cpp](../geometry.cpp) | `dot` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `equal` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `inCircle` | オンライン結果待ち | [aoj/geo_incircle: 未提出](aoj/submissions/geo_incircle.cc) |
| [geometry.cpp](../geometry.cpp) | `isContained` | オンライン結果待ち | [aoj/geo_contained: 未提出](aoj/submissions/geo_contained.cc) |
| [geometry.cpp](../geometry.cpp) | `isConvex` | オンライン結果待ち | [aoj/geo_convex: 未提出](aoj/submissions/geo_convex.cc) |
| [geometry.cpp](../geometry.cpp) | `isIntersect(Circle,Circle)` | オンライン結果待ち | [aoj/geo_circle_relation: 未提出](aoj/submissions/geo_circle_relation.cc) |
| [geometry.cpp](../geometry.cpp) | `isIntersect(Line,Line)` | オンライン結果待ち | [aoj/geo_segments_intersect: 未提出](aoj/submissions/geo_segments_intersect.cc) |
| [geometry.cpp](../geometry.cpp) | `isOrthogonal` | オンライン結果待ち | [aoj/geo_parallel: 未提出](aoj/submissions/geo_parallel.cc) |
| [geometry.cpp](../geometry.cpp) | `isParallel` | オンライン結果待ち | [aoj/geo_parallel: 未提出](aoj/submissions/geo_parallel.cc) |
| [geometry.cpp](../geometry.cpp) | `normalVector` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `onLine` | オンライン結果待ち | [aoj/geo_orientation: 未提出](aoj/submissions/geo_orientation.cc) |
| [geometry.cpp](../geometry.cpp) | `onSegment` | オンライン結果待ち | [aoj/geo_orientation: 未提出](aoj/submissions/geo_orientation.cc) |
| [geometry.cpp](../geometry.cpp) | `projection` | オンライン結果待ち | [aoj/geo_projection: 未提出](aoj/submissions/geo_projection.cc) |
| [geometry.cpp](../geometry.cpp) | `radianToDegree` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `reflection` | オンライン結果待ち | [aoj/geo_reflection: 未提出](aoj/submissions/geo_reflection.cc) |
| [geometry.cpp](../geometry.cpp) | `rotate` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `sgn` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [geometry.cpp](../geometry.cpp) | `tangent` | オンライン結果待ち | [aoj/geo_tangents: 未提出](aoj/submissions/geo_tangents.cc) |
| [geometry.cpp](../geometry.cpp) | `tangentToCircle` | オンライン結果待ち | [aoj/geo_tangent_point: 未提出](aoj/submissions/geo_tangent_point.cc) |
| [geometry.cpp](../geometry.cpp) | `unitVector` | AC確認あり | [geometry/geo_hull: AC](https://judge.yosupo.jp/submission/404465) |
| [hld.cpp](../hld.cpp) | `HeavyLightDecomposition` | AC確認あり | [previous/hld_path_sum: AC](https://judge.yosupo.jp/submission/404406) |
| [hld.cpp](../hld.cpp) | `ascend` | AC確認あり | [previous/hld_path_sum: AC](https://judge.yosupo.jp/submission/404406) |
| [hld.cpp](../hld.cpp) | `descend` | AC確認あり | [previous/hld_path_sum: AC](https://judge.yosupo.jp/submission/404406) |
| [hld.cpp](../hld.cpp) | `dfs_hld` | AC確認あり | [previous/hld_path_sum: AC](https://judge.yosupo.jp/submission/404406) |
| [hld.cpp](../hld.cpp) | `dfs_sz` | AC確認あり | [previous/hld_path_sum: AC](https://judge.yosupo.jp/submission/404406) |
| [hld.cpp](../hld.cpp) | `dist` | AC確認あり | [lc/hld_jump: AC](https://judge.yosupo.jp/submission/404454) |
| [hld.cpp](../hld.cpp) | `get_edge` | AC確認あり | [lc/hld_jump: AC](https://judge.yosupo.jp/submission/404454) |
| [hld.cpp](../hld.cpp) | `lca` | AC確認あり | [previous/hld_path_sum: AC](https://judge.yosupo.jp/submission/404406) |
| [hld.cpp](../hld.cpp) | `level_ancestor` | AC確認あり | [lc/hld_jump: AC](https://judge.yosupo.jp/submission/404454) |
| [hld.cpp](../hld.cpp) | `path_noncommutative_query` | AC確認あり | [previous/hld_composite: AC](https://judge.yosupo.jp/submission/404438) |
| [hld.cpp](../hld.cpp) | `path_query` | AC確認あり | [previous/hld_path_sum: AC](https://judge.yosupo.jp/submission/404406) |
| [hld_short.cpp](../hld_short.cpp) | `dfs_hld` | AC確認あり | [previous/hld_short: AC](https://judge.yosupo.jp/submission/404407) |
| [hld_short.cpp](../hld_short.cpp) | `dfs_sz` | AC確認あり | [previous/hld_short: AC](https://judge.yosupo.jp/submission/404407) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `all` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `all_prod` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `apply(a,b,f)` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `apply(a,b,f,k,l,r)` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `apply(p,f)` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `get` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `lazy_segtree(n)` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `lazy_segtree(vector)` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `max_right` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[lc/lazy_segtree_range_search: AC](https://judge.yosupo.jp/submission/404446) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `min_left` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[lc/lazy_segtree_range_search: AC](https://judge.yosupo.jp/submission/404446) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `ml` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[lc/lazy_segtree_range_search: AC](https://judge.yosupo.jp/submission/404446) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `mr` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[lc/lazy_segtree_range_search: AC](https://judge.yosupo.jp/submission/404446) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `prod(a,b)` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `prod(a,b,k,l,r)` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `push` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `set(p,x)` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lazy_segtree.cpp](../lazy_segtree.cpp) | `set(p,x,k,l,r)` | AC確認あり | [lc/lazy_segtree_search: AC](https://judge.yosupo.jp/submission/404445)<br>[previous/lazy_segtree: AC](https://judge.yosupo.jp/submission/404408) |
| [lcp_array.cpp](../lcp_array.cpp) | `lcp_array` | AC確認あり | [previous/lcp_array: AC](https://judge.yosupo.jp/submission/404409) |
| [linear_equation.cpp](../linear_equation.cpp) | `GaussElimination` | AC確認あり | [previous/linear_equation_det: AC](https://judge.yosupo.jp/submission/404412)<br>[previous/linear_equation_rank: AC](https://judge.yosupo.jp/submission/404413)<br>[previous/linear_equation: AC](https://judge.yosupo.jp/submission/404410) |
| [linear_equation.cpp](../linear_equation.cpp) | `LinearEquation` | AC確認あり | [previous/linear_equation: AC](https://judge.yosupo.jp/submission/404410) |
| [linear_equation_bitset.cpp](../linear_equation_bitset.cpp) | `LinearEquation<MAX>` | AC確認あり | [previous/linear_equation_bitset: AC](https://judge.yosupo.jp/submission/404414) |
| [manacher.cpp](../manacher.cpp) | `manacher` | AC確認あり | [previous/manacher: AC](https://judge.yosupo.jp/submission/404415) |
| [math.cpp](../math.cpp) | `crt` | オンライン結果待ち | [aoj/math_crt: 未提出](aoj/submissions/math_crt.cc)<br>[aoj/math_power: 未提出](aoj/submissions/math_power.cc) |
| [math.cpp](../math.cpp) | `floor_sum` | AC確認あり | [previous/math: AC](https://judge.yosupo.jp/submission/404416)<br>[aoj/math_power: 未提出](aoj/submissions/math_power.cc) |
| [math.cpp](../math.cpp) | `igcd` | オンライン結果待ち | [aoj/math_bezout: 未提出](aoj/submissions/math_bezout.cc)<br>[aoj/math_power: 未提出](aoj/submissions/math_power.cc) |
| [math.cpp](../math.cpp) | `inv_mod` | オンライン結果待ち | [aoj/math_bezout: 未提出](aoj/submissions/math_bezout.cc)<br>[aoj/math_power: 未提出](aoj/submissions/math_power.cc) |
| [math.cpp](../math.cpp) | `pow_mod` | オンライン結果待ち | [aoj/math_power: 未提出](aoj/submissions/math_power.cc) |
| [math.cpp](../math.cpp) | `smod` | オンライン結果待ち | [aoj/math_power: 未提出](aoj/submissions/math_power.cc) |
| [maxflow.cpp](../maxflow.cpp) | `add_edge` | AC確認あり | [lc/maxflow_helpers: AC](https://judge.yosupo.jp/submission/404457) |
| [maxflow.cpp](../maxflow.cpp) | `change_edge` | AC確認あり | [lc/maxflow_helpers: AC](https://judge.yosupo.jp/submission/404457)<br>[lc/maxflow_nonzero_change: AC](https://judge.yosupo.jp/submission/404473) |
| [maxflow.cpp](../maxflow.cpp) | `edges` | AC確認あり | [lc/maxflow_helpers: AC](https://judge.yosupo.jp/submission/404457) |
| [maxflow.cpp](../maxflow.cpp) | `flow(s,t)` | AC確認あり | [lc/maxflow_helpers: AC](https://judge.yosupo.jp/submission/404457) |
| [maxflow.cpp](../maxflow.cpp) | `flow(s,t,limit)` | AC確認あり | [lc/maxflow_helpers: AC](https://judge.yosupo.jp/submission/404457) |
| [maxflow.cpp](../maxflow.cpp) | `get_edge` | AC確認あり | [lc/maxflow_helpers: AC](https://judge.yosupo.jp/submission/404457) |
| [maxflow.cpp](../maxflow.cpp) | `mf_graph` | AC確認あり | [lc/maxflow_helpers: AC](https://judge.yosupo.jp/submission/404457) |
| [maxflow.cpp](../maxflow.cpp) | `min_cut` | AC確認あり | [lc/maxflow_helpers: AC](https://judge.yosupo.jp/submission/404457) |
| [mincostflow.cpp](../mincostflow.cpp) | `add_edge` | TLE | [lc/mincost_slope_assignment: TLE](https://judge.yosupo.jp/submission/404478)<br>[aoj/mincost_slope: 未提出](aoj/submissions/mincost_slope.cc)<br>[aoj/mincost_slope_unlimited: 未提出](aoj/submissions/mincost_slope_unlimited.cc) |
| [mincostflow.cpp](../mincostflow.cpp) | `edges` | TLE | [lc/mincost_slope_assignment: TLE](https://judge.yosupo.jp/submission/404478)<br>[aoj/mincost_slope: 未提出](aoj/submissions/mincost_slope.cc)<br>[aoj/mincost_slope_unlimited: 未提出](aoj/submissions/mincost_slope_unlimited.cc) |
| [mincostflow.cpp](../mincostflow.cpp) | `flow(s,t)` | TLE | [lc/mincost_slope_assignment: TLE](https://judge.yosupo.jp/submission/404478)<br>[aoj/mincost_slope: 未提出](aoj/submissions/mincost_slope.cc)<br>[aoj/mincost_slope_unlimited: 未提出](aoj/submissions/mincost_slope_unlimited.cc) |
| [mincostflow.cpp](../mincostflow.cpp) | `flow(s,t,limit)` | TLE | [lc/mincost_slope_assignment: TLE](https://judge.yosupo.jp/submission/404478)<br>[aoj/mincost_slope: 未提出](aoj/submissions/mincost_slope.cc)<br>[aoj/mincost_slope_unlimited: 未提出](aoj/submissions/mincost_slope_unlimited.cc) |
| [mincostflow.cpp](../mincostflow.cpp) | `get_edge` | TLE | [lc/mincost_slope_assignment: TLE](https://judge.yosupo.jp/submission/404478)<br>[aoj/mincost_slope: 未提出](aoj/submissions/mincost_slope.cc)<br>[aoj/mincost_slope_unlimited: 未提出](aoj/submissions/mincost_slope_unlimited.cc) |
| [mincostflow.cpp](../mincostflow.cpp) | `mcf_graph` | TLE | [lc/mincost_slope_assignment: TLE](https://judge.yosupo.jp/submission/404478)<br>[aoj/mincost_slope: 未提出](aoj/submissions/mincost_slope.cc)<br>[aoj/mincost_slope_unlimited: 未提出](aoj/submissions/mincost_slope_unlimited.cc) |
| [mincostflow.cpp](../mincostflow.cpp) | `slope(s,t)` | TLE | [lc/mincost_slope_assignment: TLE](https://judge.yosupo.jp/submission/404478)<br>[aoj/mincost_slope: 未提出](aoj/submissions/mincost_slope.cc)<br>[aoj/mincost_slope_unlimited: 未提出](aoj/submissions/mincost_slope_unlimited.cc) |
| [mincostflow.cpp](../mincostflow.cpp) | `slope(s,t,limit)` | TLE | [lc/mincost_slope_assignment: TLE](https://judge.yosupo.jp/submission/404478)<br>[aoj/mincost_slope: 未提出](aoj/submissions/mincost_slope.cc)<br>[aoj/mincost_slope_unlimited: 未提出](aoj/submissions/mincost_slope_unlimited.cc) |
| [mo.cpp](../mo.cpp) | `Mo` | AC確認あり | [previous/mo: AC](https://judge.yosupo.jp/submission/404420)<br>[lc/mo_directional: AC](https://judge.yosupo.jp/submission/404471) |
| [mo.cpp](../mo.cpp) | `add` | AC確認あり | [previous/mo: AC](https://judge.yosupo.jp/submission/404420)<br>[lc/mo_directional: AC](https://judge.yosupo.jp/submission/404471) |
| [mo.cpp](../mo.cpp) | `build(AL,AR,EL,ER,out)` | AC確認あり | [previous/mo: AC](https://judge.yosupo.jp/submission/404420)<br>[lc/mo_directional: AC](https://judge.yosupo.jp/submission/404471) |
| [mo.cpp](../mo.cpp) | `build(add,erase,out)` | AC確認あり | [previous/mo: AC](https://judge.yosupo.jp/submission/404420)<br>[lc/mo_directional: AC](https://judge.yosupo.jp/submission/404471) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `convolution` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `inv` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `mint` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `mod` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `ntt` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `operator*` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `operator*=` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `operator+` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `operator+=` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `operator-` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `operator-=` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `operator/` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `operator/=` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `pow` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [modint_convolution.cpp](../modint_convolution.cpp) | `val` | AC確認あり | [lc/mint_all: AC](https://judge.yosupo.jp/submission/404461) |
| [rerooting.cpp](../rerooting.cpp) | `RerootingDP` | AC確認あり | [previous/rerooting: AC](https://judge.yosupo.jp/submission/404436)<br>[lc/rerooting_nonzero_root: AC](https://judge.yosupo.jp/submission/404472) |
| [rerooting.cpp](../rerooting.cpp) | `add_edge` | AC確認あり | [previous/rerooting: AC](https://judge.yosupo.jp/submission/404436)<br>[lc/rerooting_nonzero_root: AC](https://judge.yosupo.jp/submission/404472) |
| [rerooting.cpp](../rerooting.cpp) | `build` | AC確認あり | [previous/rerooting: AC](https://judge.yosupo.jp/submission/404436)<br>[lc/rerooting_nonzero_root: AC](https://judge.yosupo.jp/submission/404472) |
| [rerooting.cpp](../rerooting.cpp) | `es_build` | AC確認あり | [previous/rerooting: AC](https://judge.yosupo.jp/submission/404436)<br>[lc/rerooting_nonzero_root: AC](https://judge.yosupo.jp/submission/404472) |
| [rerooting.cpp](../rerooting.cpp) | `outs_start` | AC確認あり | [previous/rerooting: AC](https://judge.yosupo.jp/submission/404436)<br>[lc/rerooting_nonzero_root: AC](https://judge.yosupo.jp/submission/404472) |
| [rerooting.cpp](../rerooting.cpp) | `reroot` | AC確認あり | [previous/rerooting: AC](https://judge.yosupo.jp/submission/404436)<br>[lc/rerooting_nonzero_root: AC](https://judge.yosupo.jp/submission/404472) |
| [rollback_mo.cpp](../rollback_mo.cpp) | `MoRollBack` | AC確認あり | [previous/rollback_mo: AC](https://judge.yosupo.jp/submission/404423) |
| [rollback_mo.cpp](../rollback_mo.cpp) | `add` | AC確認あり | [previous/rollback_mo: AC](https://judge.yosupo.jp/submission/404423) |
| [rollback_mo.cpp](../rollback_mo.cpp) | `run` | AC確認あり | [previous/rollback_mo: AC](https://judge.yosupo.jp/submission/404423) |
| [rolling_hash.cpp](../rolling_hash.cpp) | `get` | AC確認あり | [previous/rolling_hash: AC](https://judge.yosupo.jp/submission/404425) |
| [rolling_hash.cpp](../rolling_hash.cpp) | `mul` | AC確認あり | [previous/rolling_hash: AC](https://judge.yosupo.jp/submission/404425) |
| [rolling_hash.cpp](../rolling_hash.cpp) | `rolling_hash` | AC確認あり | [previous/rolling_hash: AC](https://judge.yosupo.jp/submission/404425) |
| [scc.cpp](../scc.cpp) | `add_edge` | AC確認あり | [previous/scc: AC](https://judge.yosupo.jp/submission/404426) |
| [scc.cpp](../scc.cpp) | `scc` | AC確認あり | [previous/scc: AC](https://judge.yosupo.jp/submission/404426) |
| [scc.cpp](../scc.cpp) | `scc_graph` | AC確認あり | [previous/scc: AC](https://judge.yosupo.jp/submission/404426) |
| [segtree.cpp](../segtree.cpp) | `all_prod` | AC確認あり | [previous/segtree: AC](https://judge.yosupo.jp/submission/404427)<br>[lc/segtree_search: AC](https://judge.yosupo.jp/submission/404443) |
| [segtree.cpp](../segtree.cpp) | `get` | AC確認あり | [previous/segtree: AC](https://judge.yosupo.jp/submission/404427)<br>[lc/segtree_search: AC](https://judge.yosupo.jp/submission/404443) |
| [segtree.cpp](../segtree.cpp) | `max_right` | AC確認あり | [lc/segtree_search: AC](https://judge.yosupo.jp/submission/404443) |
| [segtree.cpp](../segtree.cpp) | `min_left` | AC確認あり | [lc/segtree_search: AC](https://judge.yosupo.jp/submission/404443) |
| [segtree.cpp](../segtree.cpp) | `ml` | AC確認あり | [lc/segtree_search: AC](https://judge.yosupo.jp/submission/404443) |
| [segtree.cpp](../segtree.cpp) | `mr` | AC確認あり | [lc/segtree_search: AC](https://judge.yosupo.jp/submission/404443) |
| [segtree.cpp](../segtree.cpp) | `prod` | AC確認あり | [previous/segtree: AC](https://judge.yosupo.jp/submission/404427)<br>[lc/segtree_search: AC](https://judge.yosupo.jp/submission/404443) |
| [segtree.cpp](../segtree.cpp) | `segtree(n)` | AC確認あり | [previous/segtree: AC](https://judge.yosupo.jp/submission/404427)<br>[lc/segtree_search: AC](https://judge.yosupo.jp/submission/404443) |
| [segtree.cpp](../segtree.cpp) | `segtree(vector)` | AC確認あり | [previous/segtree: AC](https://judge.yosupo.jp/submission/404427)<br>[lc/segtree_search: AC](https://judge.yosupo.jp/submission/404443) |
| [segtree.cpp](../segtree.cpp) | `set` | AC確認あり | [previous/segtree: AC](https://judge.yosupo.jp/submission/404427)<br>[lc/segtree_search: AC](https://judge.yosupo.jp/submission/404443) |
| [suffix_array.cpp](../suffix_array.cpp) | `suffix_array(string)` | AC確認あり | [previous/suffix_array: AC](https://judge.yosupo.jp/submission/404429) |
| [suffix_array.cpp](../suffix_array.cpp) | `suffix_array(vector<int>,upper)` | AC確認あり | [lc/suffix_array_integer: AC](https://judge.yosupo.jp/submission/404450) |
| [suffix_array.cpp](../suffix_array.cpp) | `suffix_array<T>(vector<T>)` | AC確認あり | [previous/suffix_array: AC](https://judge.yosupo.jp/submission/404429) |
| [template.cpp](../template.cpp) | `chmax` | AC確認あり | [previous/template: AC](https://judge.yosupo.jp/submission/404430) |
| [template.cpp](../template.cpp) | `chmin` | AC確認あり | [previous/template: AC](https://judge.yosupo.jp/submission/404430) |
| [template.cpp](../template.cpp) | `main` | WA | [lc/template_entry: WA](https://judge.yosupo.jp/submission/404462) |
| [template.cpp](../template.cpp) | `solve` | WA | [lc/template_entry: WA](https://judge.yosupo.jp/submission/404462) |
| [trie.cpp](../trie.cpp) | `N::N` | AC確認あり | [previous/trie: AC](https://judge.yosupo.jp/submission/404431) |
| [trie.cpp](../trie.cpp) | `find` | AC確認あり | [previous/trie: AC](https://judge.yosupo.jp/submission/404431) |
| [trie.cpp](../trie.cpp) | `insert` | AC確認あり | [previous/trie: AC](https://judge.yosupo.jp/submission/404431) |
| [twosat.cpp](../twosat.cpp) | `add` | AC確認あり | [previous/twosat: AC](https://judge.yosupo.jp/submission/404432) |
| [twosat.cpp](../twosat.cpp) | `add_clause` | AC確認あり | [previous/twosat: AC](https://judge.yosupo.jp/submission/404432) |
| [twosat.cpp](../twosat.cpp) | `answer` | AC確認あり | [previous/twosat: AC](https://judge.yosupo.jp/submission/404432) |
| [twosat.cpp](../twosat.cpp) | `satisfiable` | AC確認あり | [previous/twosat: AC](https://judge.yosupo.jp/submission/404432) |
| [twosat.cpp](../twosat.cpp) | `two_sat` | AC確認あり | [previous/twosat: AC](https://judge.yosupo.jp/submission/404432) |
| [wavelet_matrix.cpp](../wavelet_matrix.cpp) | `WaveletMatrix` | AC確認あり | [lc/wavelet_all: AC](https://judge.yosupo.jp/submission/404441)<br>[lc/wavelet_frequency: AC](https://judge.yosupo.jp/submission/404442) |
| [wavelet_matrix.cpp](../wavelet_matrix.cpp) | `access` | AC確認あり | [lc/wavelet_all: AC](https://judge.yosupo.jp/submission/404441)<br>[lc/wavelet_frequency: AC](https://judge.yosupo.jp/submission/404442) |
| [wavelet_matrix.cpp](../wavelet_matrix.cpp) | `kth` | AC確認あり | [lc/wavelet_all: AC](https://judge.yosupo.jp/submission/404441)<br>[lc/wavelet_frequency: AC](https://judge.yosupo.jp/submission/404442) |
| [wavelet_matrix.cpp](../wavelet_matrix.cpp) | `kth_largest` | AC確認あり | [lc/wavelet_all: AC](https://judge.yosupo.jp/submission/404441)<br>[lc/wavelet_frequency: AC](https://judge.yosupo.jp/submission/404442) |
| [wavelet_matrix.cpp](../wavelet_matrix.cpp) | `less` | AC確認あり | [lc/wavelet_all: AC](https://judge.yosupo.jp/submission/404441)<br>[lc/wavelet_frequency: AC](https://judge.yosupo.jp/submission/404442) |
| [wavelet_matrix.cpp](../wavelet_matrix.cpp) | `next_value` | AC確認あり | [lc/wavelet_all: AC](https://judge.yosupo.jp/submission/404441)<br>[lc/wavelet_frequency: AC](https://judge.yosupo.jp/submission/404442) |
| [wavelet_matrix.cpp](../wavelet_matrix.cpp) | `prev_value` | AC確認あり | [lc/wavelet_all: AC](https://judge.yosupo.jp/submission/404441)<br>[lc/wavelet_frequency: AC](https://judge.yosupo.jp/submission/404442) |
| [wavelet_matrix.cpp](../wavelet_matrix.cpp) | `range_freq` | AC確認あり | [lc/wavelet_all: AC](https://judge.yosupo.jp/submission/404441)<br>[lc/wavelet_frequency: AC](https://judge.yosupo.jp/submission/404442) |
| [wavelet_matrix.cpp](../wavelet_matrix.cpp) | `rank(x,l,r)` | AC確認あり | [lc/wavelet_all: AC](https://judge.yosupo.jp/submission/404441)<br>[lc/wavelet_frequency: AC](https://judge.yosupo.jp/submission/404442) |
| [wavelet_matrix.cpp](../wavelet_matrix.cpp) | `rank(x,r)` | AC確認あり | [lc/wavelet_all: AC](https://judge.yosupo.jp/submission/404441)<br>[lc/wavelet_frequency: AC](https://judge.yosupo.jp/submission/404442) |
| [z_algorithm.cpp](../z_algorithm.cpp) | `z_algorithm` | AC確認あり | [previous/z_algorithm: AC](https://judge.yosupo.jp/submission/404435) |

## 検証方法と注意点

- 元の 32 個の `.cpp` は変更していません。全文または指定した元ファイルの範囲の SHA-256 を manifest に記録しています。
- `geometry.cpp` は、コンパイルできない `circumCircle` を除いた元の本文をコピーし、前方宣言を足して関数別に提出します。`circumCircle` は別の提出で元の本文のコンパイル結果を確認します。元ファイル全体をそのまま使用した場合の CE は残っています。
- `segtree` の境界探索は Predecessor Problem の出力に使用。`lazy_segtree` は同じ問題に加え、アフィン更新後に区間長を条件として探索し、受理した区間の和から公式問題の答えを出しています。
- Wavelet Matrix は `kth_largest` で公式の k 番目を出力し、`access` は入力配列と照合。前後の値・出現回数・範囲内個数は公式入力上の整合性検査と、小区間のソートによる独立検査を行います。`range_freq` と 2 引数 `rank` には出現回数問題の提出もあります。
- 最小費用流は、最大 7 辺・容量 0〜2 の固定乱数グラフ 150 個について全辺の流量を全探索。`slope` の全整数流量での費用、折れ点の順序、同傾き区間の併合、流量上限、全 flow/slope オーバーロード、`edges`・`get_edge` と保存則を同じ提出内で検査しています。AOJ では折れ線から指定流量の費用を補間して出力します。
- `change_edge` は 0 以外の既存フローも設定してから追加フローを計算。Mo の左右別コールバックは小配列の転倒数全探索と比較。全方位木 DP は根を `n-1` にした提出も追加しています。
- `mint` の演算は ACL との固定乱数比較も実行。`BMBM` は得られた漸化式の逐次計算と比較。これらの補助検査もリンク先の提出ソースに含まれます。
- 数論の AOJ 提出には負数・非互いに素・解のない CRT・負係数の floor_sum の小規模全探索を含めています。CRT は拡張 Euclid 問題でも、`x ≡ 0 (mod a), x ≡ gcd(a,b) (mod b)` を解いて出力に使います。
- `template.cpp` の `chmin`・`chmax` は前回 AC。今回は元の `main`・`solve` もそのまま提出したため、空の `solve()` が出力しないことにより A+B は WA です。これは未実装のテンプレートの振る舞いです。
- 前回の `mincostflow.cpp` は Assignment Problem で TLE。今回の追加提出でも、本体の修正や他の実装への差し替えは行っていません。
- 今回の `mincost_slope_assignment` も Assignment Problem で TLE になりました。表中でこの提出に対応する関数に TLE と表示されるのは、提出全体の結果です。各補助関数が個別に時間超過したという意味ではありません。より小さい制約の AOJ GRL_6_B 用コードも用意してあり、認証後に提出できます。

## AOJ 提出の再開

AOJ の結果 URL には認証済みの提出が必要です。まだ認証ファイルがない場合は、手元のターミナルで次を実行してください。パスワードは保存せず、提出用セッションのみ一時フォルダに保存します。

```sh
python3 library_checker/setup_aoj_auth.py
python3 library_checker/submit_aoj.py
python3 library_checker/make_function_report.py
```

再生成順序: `build_full.py` → `build_aoj.py` → `add_slope_submission.py` → `function_inventory.py`。Library Checker の追加提出は `submit.py library_checker/full` と `submit.py library_checker/full_geometry` で再開できます。
