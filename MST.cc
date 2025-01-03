#include "Graph.hh"
#include "PQ.hh"
#include "UF.hh"

struct BoruvkaMST {
  ns::deque<Edge> mst;
  int wt{0};

  BoruvkaMST(const EdgeWeightedGraph &G) {
    UF uf(G.V);
    for (int t = 1; t < G.V && mst.size() < G.V - 1; t += t) {
      ns::deque<const Edge *> closest(G.V, nullptr);
      for (const auto &e : G.edges()) {
        int v{e.v}, w{e.w};
        int i{uf.find(v)}, j{uf.find(w)};
        if (i == j)
          continue;
        if (closest[i] == nullptr || e < *closest[i])
          closest[i] = &e;
        if (closest[j] == nullptr || e < *closest[j])
          closest[j] = &e;
      }
      for (int i = 0; i < G.V; i++) {
        const Edge *e{closest[i]};
        if (e) {
          int v{e->v}, w{e->w};
          if (uf.find(v) != uf.find(w)) {
            mst.push_back(*e);
            wt += e->weight;
            uf.join(v, w);
          }
        }
      }
    }
  }

  auto edges() const { return mst; }
  int weight() const { return wt; }
};

struct KruskalMST {
  int wt{0};
  ns::deque<Edge> mst;

  KruskalMST(const EdgeWeightedGraph &G) {
    MinPQ<Edge> pq(G.edges());
    UF uf(G.V);
    while (!pq.empty() && mst.size() < G.V - 1) {
      Edge e{pq.delMin()};
      int rootV{uf.find(e.v)}, rootW{uf.find(e.w)};
      if (rootV == rootW)
        continue;
      uf.join(rootV, rootW);
      mst.push_back(e), wt += e.weight;
    }
  }

  ns::deque<Edge> edges() const { return mst; }
  int weight() const { return wt; }
};

// priority first search minimun spanning forest
struct PrimMST {
  ns::deque<Edge *> edgeTo;
  ns::deque<int> distTo;
  ns::deque<bool> marked;
  IndexMinPQ<int> pq;

  PrimMST(const EdgeWeightedGraph &G)
      : edgeTo(G.V, nullptr), distTo(G.V, 0xffff), marked(G.V, false), pq(G.V) {
    for (int v = 0; v < G.V; ++v)
      if (!marked[v])
        search(G, v);
  }

  void search(const EdgeWeightedGraph &G, int s) {
    distTo[s] = 0;
    pq.insert(s, distTo[s]);
    while (!pq.empty()) {
      int v{pq.delMin()};
      scan(G, v);
    }
  }

  void scan(const EdgeWeightedGraph &G, int v) {
    marked[v] = true;
    for (const auto &e : G.adj[v]) {
      int w{e.other(v)};
      if (marked[w])
        continue;
      if (e.weight < distTo[w]) {
        distTo[w] = e.weight;
        delete edgeTo[w];
        edgeTo[w] = new Edge(e);
        if (pq.contains(w))
          pq.decreaseKey(w, distTo[w]);
        else
          pq.insert(w, distTo[w]);
      }
    }
  }

  ~PrimMST() {
    for (auto e : edgeTo)
      delete e;
  }

  ns::deque<Edge> edges() const {
    ns::deque<Edge> mst;
    for (int v(0), sz(edgeTo.size()); v < sz; ++v)
      if (auto e{edgeTo[v]})
        mst.push_back(*e);
    return mst;
  }

  int weight() const {
    int weight = 0;
    for (const auto &e : edges())
      weight += e.weight;
    return weight;
  }
};

struct LazyPrimMST {
  int wt;
  ns::deque<Edge> mst;
  ns::deque<bool> marked;
  MinPQ<Edge> pq;

  LazyPrimMST(const EdgeWeightedGraph &G) : wt{0}, marked(G.V, false), pq(G.V) {
    for (int v = 0; v < G.V; ++v)
      if (!marked[v])
        search(G, v);
  }
  void search(const EdgeWeightedGraph &G, int s) {
    scan(G, s);
    while (!pq.empty()) {
      Edge e{pq.delMin()};
      int v{e.v}, w{e.w};
      if (marked[v] && marked[w])
        continue;
      mst.push_back(e);
      wt += e.weight;
      if (!marked[v])
        scan(G, v);
      if (!marked[w])
        scan(G, w);
    }
  }

  void scan(const EdgeWeightedGraph &G, int v) {
    marked[v] = true;
    for (const auto &e : G.adj[v])
      if (!marked[e.other(v)])
        pq.insert(e);
  }

  ns::deque<Edge> edges() const { return mst; }
  int weight() const { return wt; }
};

template <class MST> void printMST(const MST &mst) {
  ns::deque<Edge> deck{mst.edges()};
  for (const auto &e : deck)
    std::print("[{}->{}|{}]\t", e.v, e.w, e.weight);
  std::print("\nweight\t{}\n", mst.weight());
}

int main() {
  constexpr int v{8}, e{8};
  for (int k = 0; k < 16; k++) {
    EdgeWeightedGraph EWG(v);
    generateGraph(EWG, e);
    std::print("EdgeWeightedGraph\n");
    printGraph(EWG);
    std::print("\n");

    std::print("BoruvkaMSTMST\n");
    KruskalMST BMST(EWG);
    printMST(BMST);
    std::print("\n");

    std::print("KruskalMST\n");
    KruskalMST KMST(EWG);
    printMST(KMST);
    std::print("\n");
    assert(BMST.weight() == KMST.weight());

    std::print("PrimMST\n");
    PrimMST PMST(EWG);
    printMST(PMST);
    std::print("\n");
    assert(KMST.weight() == PMST.weight());

    std::print("LazyPrimMST\n");
    LazyPrimMST LPMST(EWG);
    printMST(LPMST);
    std::print("\n");
    assert(PMST.weight() == LPMST.weight());
  }
}
