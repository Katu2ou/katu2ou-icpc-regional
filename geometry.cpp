using ld=long double;
using Point=complex<ld>; using pt=Point;
const ld EPS=1e-10L,PI=acosl(-1.L);

inline bool equal(const ld&a,const ld&b){ //a,bが誤差を許して等しいか
    return fabsl(a-b)<EPS;
}
inline int sgn(const ld&x){ //xの符号を-1,0,1で返す
    return (x>EPS)-(x<-EPS);
}
Point unitVector(const Point&a){ //aと同方向の単位ベクトル
    assert(abs(a)>EPS); return a/abs(a);
}
Point normalVector(const Point&a){ //aを反時計回りに90度回転
    return a*Point(0,1);
}
ld dot(const Point&a,const Point&b){ //内積
    return real(conj(a)*b);
}
ld cross(const Point&a,const Point&b){ //外積
    return imag(conj(a)*b);
}
Point rotate(const Point&p,ld t){ //pを反時計回りにt(rad)回転
    return p*polar<ld>(1,t);
}
ld radianToDegree(ld x){ //radを度に変換
    return x*180/PI;
}
ld degreeToRadian(ld x){ //度をradに変換
    return x*PI/180;
}

struct Line{
    Point a,b;
    Line()=default;
    Line(Point x,Point y):a(x),b(y){} //2点x,yを通る直線(線分)
    Line(ld A,ld B,ld C){ //Ax+By=Cを表す直線
        assert(!equal(A,0)||!equal(B,0));
        if(!equal(B,0))a={0,C/B},b={1,(C-A)/B};
        else a={C/A,0},b={C/A,1};
    }
};

struct Circle{
    Point p; ld r;
    Circle()=default;
    Circle(Point x,ld y):p(x),r(y){} //中心x,半径yの円
};

bool onLine(const Line&l,const Point&p){ //pが直線l上か
    return fabsl(cross(l.b-l.a,p-l.a))<=EPS;
}

bool onSegment(const Line&l,const Point&p){ //pが線分l上か
    return ccw(l.a,l.b,p)==0;
}

Circle circumCircle(Point a,Point b,Point c){ //三角形abcの外接円
    assert(fabsl(cross(b-a,c-a))>EPS);
    Point x=(a+b)/2,y=(a+c)/2;
    Line l={x,x+normalVector(b-a)};
    Line m={y,y+normalVector(c-a)};
    Point p=crossPoint(l,m);
    return {p,abs(p-a)};
}

Point projection(const Line&l,const Point&p){ //lへpから下ろした垂線の足
    Point d=l.b-l.a;
    assert(abs(d)>EPS);
    return l.a+d*dot(p-l.a,d)/norm(d);
}
Point reflection(const Line&l,const Point&p){ //lに関するpの対称点
    return 2.L*projection(l,p)-p;
}

int ccw(const Point&a,Point b,Point c){ //a,b,cの位置関係を判定
    b-=a;c-=a; ld x=cross(b,c);
    if(abs(x)>EPS)return x>0?1:-1; //反時計/時計
    if(dot(b,c)<-EPS)return 2;      //c-a-b
    if(norm(b)+EPS<norm(c))return -2; //a-b-c
    return 0;                       //cが線分ab上
}

bool isOrthogonal(const Line&a,const Line&b){ //2直線が直交するか
    return equal(dot(a.b-a.a,b.b-b.a),0);
}
bool isParallel(const Line&a,const Line&b){ //2直線が平行か
    return equal(cross(a.b-a.a,b.b-b.a),0);
}
bool isIntersect(const Line&a,const Line&b){ //2線分が交差するか
    return ccw(a.a,a.b,b.a)*ccw(a.a,a.b,b.b)<=0
        && ccw(b.a,b.b,a.a)*ccw(b.a,b.b,a.b)<=0;
}

Point crossPoint(const Line&a,const Line&b){ //2直線の交点
    Point d=a.b-a.a,e=b.b-b.a;
    ld z=cross(d,e),w=cross(d,b.a-a.a);
    if(equal(z,0)){assert(equal(w,0));return b.a;}
    return a.a+d*cross(b.a-a.a,e)/z;
}

ld distanceBetweenLineAndPoint(const Line&l,const Point&p){ //直線lと点pの距離
    Point d=l.b-l.a;
    assert(abs(d)>EPS);
    return fabsl(cross(d,p-l.a))/abs(d);
}
ld distanceBetweenSegmentAndPoint(const Line&l,const Point&p){ //線分lと点pの距離
    Point d=l.b-l.a;
    assert(abs(d)>EPS);
    if(dot(d,p-l.a)<EPS)return abs(p-l.a);
    if(dot(-d,p-l.b)<EPS)return abs(p-l.b);
    return fabsl(cross(d,p-l.a))/abs(d);
}
ld distanceBetweenSegments(const Line&a,const Line&b){ //2線分間の距離
    if(isIntersect(a,b))return 0;
    return min({
        distanceBetweenSegmentAndPoint(a,b.a),
        distanceBetweenSegmentAndPoint(a,b.b),
        distanceBetweenSegmentAndPoint(b,a.a),
        distanceBetweenSegmentAndPoint(b,a.b)
    });
}

ld SignedPolygonArea(const vector<Point>&p){ //多角形の符号付き面積
    ld s=0;
    for(int i=0,n=p.size();i<n;i++)
        s+=cross(p[i],p[(i+1)%n]);
    return s/2;
}
ld PolygonArea(const vector<Point>&p){ //多角形の面積
    return fabsl(SignedPolygonArea(p));
}

bool isConvex(const vector<Point>&p){ //多角形が凸か
    if(p.size()<3)return false;
    int s=0;
    for(int i=0,n=p.size();i<n;i++){
        int t=sgn(cross(p[(i+1)%n]-p[i],
                        p[(i+2)%n]-p[(i+1)%n]));
        if(t&&s&&t!=s)return false;
        if(t)s=t;
    }
    return true;
}

int isContained(const vector<Point>&g,const Point&p){ //点pが多角形gの内/辺上/外か(2/1/0)
    bool in=0;
    for(int i=0,n=g.size();i<n;i++){
        Point a=g[i]-p,b=g[(i+1)%n]-p;
        if(imag(a)>imag(b))swap(a,b);
        if(imag(a)<=EPS&&EPS<imag(b)&&cross(a,b)<-EPS)in^=1;
        if(equal(cross(a,b),0)&&dot(a,b)<=EPS)return 1;
    }
    return in?2:0;
}

vector<Point> ConvexHull(vector<Point>p,bool col=false){ //凸包を反時計回りに返す(col=trueなら辺上も含む)
    sort(p.begin(),p.end(),[](Point a,Point b){
        return real(a)!=real(b)?real(a)<real(b):imag(a)<imag(b);
    });
    p.erase(unique(p.begin(),p.end(),[](Point a,Point b){
        return abs(a-b)<EPS;
    }),p.end());

    int n=p.size(),k=0;
    if(n<=1)return p;
    vector<Point>h(2*n);
    if(col){
        bool all=1;
        for(int i=2;i<n;i++)
            if(fabsl(cross(p[1]-p[0],p[i]-p[0]))>EPS) all=0;
        if(all)return p;
    }

    auto bad=[&](Point a,Point b,Point c){ //凸包に不要な中間点か
        ld x=cross(b-a,c-b);
        return col?x<-EPS:x<EPS;
    };

    for(auto x:p){
        while(k>=2&&bad(h[k-2],h[k-1],x))--k;
        h[k++]=x;
    }
    for(int i=n-2,t=k+1;i>=0;i--){
        while(k>=t&&bad(h[k-2],h[k-1],p[i]))--k;
        h[k++]=p[i];
    }
    h.resize(k-1);
    return h;
}

int isIntersect(const Circle&a,const Circle&b){ //2円の位置関係(返り値=共通接線数,-1は同一円)
    ld d=abs(a.p-b.p),s=a.r+b.r,t=fabsl(a.r-b.r);
    if(equal(d,0)&&equal(a.r,b.r))return -1;
    if(d>s+EPS)return 4; //離れている
    if(equal(d,s))return 3; //外接
    if(d<t-EPS)return 0; //内包
    if(equal(d,t))return 1; //内接
    return 2; //2点で交わる
}

Circle inCircle(const Point&a,const Point&b,const Point&c){ //三角形abcの内接円
    ld A=abs(b-c),B=abs(a-c),C=abs(a-b);
    assert(A+B+C>EPS&&fabsl(cross(b-a,c-a))>EPS);
    Point p=(a*A+b*B+c*C)/(A+B+C);
    return {p,distanceBetweenLineAndPoint({a,b},p)};
}

vector<Point> crossPoint(const Circle&c,const Line&l){ //円cと直線lの交点
    ld d=distanceBetweenLineAndPoint(l,c.p);
    if(d>c.r+EPS)return {};
    Point h=projection(l,c.p);
    if(equal(d,c.r))return {h};
    Point u=unitVector(l.b-l.a)
           *sqrt(max<ld>(0,c.r*c.r-d*d));
    return {h-u,h+u};
}

vector<Point> crossPoint(const Circle&a,const Circle&b){ //2円の交点
    int m=isIntersect(a,b);
    if(m==4||m==0||m==-1)return {};

    ld d=abs(b.p-a.p);
    ld x=(a.r*a.r+d*d-b.r*b.r)/(2*d);
    ld y=sqrt(max<ld>(0,a.r*a.r-x*x));

    Point u=(b.p-a.p)/d,h=a.p+u*x;
    if(m==1||m==3)return {h};

    Point v=u*Point(0,y);
    return {h+v,h-v};
}

vector<Point> tangentToCircle(const Point&p,const Circle&c){ //点pから円cへの接点
    ld d=norm(c.p-p);
    if(d<c.r*c.r-EPS)return {};
    return crossPoint(c,Circle(p,sqrt(max<ld>(0,d-c.r*c.r))));
}

vector<Line> tangent(const Circle&a,const Circle&b){ //2円の共通接線
    vector<Line>r;
    ld g=abs(a.p-b.p);
    if(equal(g,0))return r;

    Point u=unitVector(b.p-a.p),v=normalVector(u);

    for(int s:{-1,1}){
        ld h=(a.r+b.r*s)/g,q=1-h*h;
        if(q<-EPS)continue;

        if(fabsl(q)<EPS){
            Point w=h>0?u:-u;
            r.push_back({a.p+w*a.r,a.p+w*a.r+v});
        }else{
            Point U=u*h,V=v*sqrt(max<ld>(0,q));
            for(int t:{1,-1}){
                Point w=U+V*(ld)t;
                r.push_back({
                    a.p+w*a.r,
                    b.p-w*(b.r*s)
                });
            }
        }
    }
    return r;
}