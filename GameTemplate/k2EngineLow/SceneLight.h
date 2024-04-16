#pragma once
#define MAX_DIRECTIONAL_LIGHT  4
#define MAX_POINT_LIGHT  256
#define MAX_SPOT_LIGHT  256

namespace nsK2EngineLow {

    //ディレクションライト構造体
    struct DirectionalLight
    {
        Vector3 direction;  // ライトの方向。
        int castShadow = true;     // 影をキャストする？、初期化せんでええんか？
        Vector3 color;      // ライトのカラー。
        float pad2;
    };

    //ポイントライト構造体
    struct SPointLight
    {
    private:
        Vector3 position;       // 座標
        int isUse = false;      // 使用中フラグ
        Vector3 color;          // ライトのカラー
        float range;            // 影響範囲
        Vector3 positionInView; // カメラ空間での座標、使い道を知らないけど後で必要になったら面倒な事になりそう置いておく
        float affectPow;        // 影響率に累乗するパラメータ。

    public:
        //座標を設定。
        void SetPosition(const Vector3& position)
        {
            this->position = position;
        }
        void SetPosition(float x, float y, float z)
        {
            SetPosition({ x,y,z });
        }

        //カラーを設定。
        void SetColor(const Vector3& color)
        {
            this->color = color;
        }
        void SetColor(float r, float g, float b)
        {
            SetColor({ r,g,b });
        }

        //光が届く範囲を設定。
        void SetRange(float range)
        {
            this->range = range;
        }
        //影響率の累乗数を設定。
        //数値を大きくするほど光の減衰の仕方が急になる。(わかんなかったら直接聞いてください)
        void SetAffectPowParam(float powParam)
        {
            affectPow = powParam;
        }

        //座標を取得。
        const Vector3& GetPosition() const
        {
            return position;
        }
        //カラーを取得。
        const Vector3& GetColor() const
        {
            return color;
        }
        //影響範囲を取得。
        float GetRange() const
        {
            return range;
        }

        //ポイントライトを使用中にする。
        //エンジン内で使う用の関数なのでゲーム側から使用しないでください。
        void Use()
        {
            isUse = true;
        }
        //ポイントライトを未使用にする。
        //エンジン内で使う用の関数なのでゲーム側から使用しないでください。
        void UnUse()
        {
            isUse = false;
        }

        //更新
        //エンジン内で使う用の関数なのでゲーム側から使用しないでください。
        void Update();
    };

    //スポットライト構造体
    struct SSpotLight {
    private:
        Vector3 position;           // 座標。
        int isUse = false;          // 使用中フラグ。
        Vector3 positionInView;     // カメラ空間での座標。
        int no = 0;                 // ライトの番号。
        Vector3 color;              // カラー
        float range;                // 影響範囲
        Vector3 direction;          // 射出方向
        float angle;                // 射出角度(単位はラジアン)
        Vector3 directionInView;    // カメラ空間での射出方向。
        float pad1;
        Vector3 pow = g_vec3One;    // 影響率に累乗するパラメータ、
                                    //xを距離による影響率に、yを角度による影響率に適用する
        float pad2;

    public:
        //スポットライトの番号を取得。
        int GetNo() const
        {
            return no;
        }
        //スポットライトの番号を設定。
        void SetNo(int no)
        {
            this->no = no;
        }

        //射出方向を設定。
        void SetDirection(const Vector3& direction)
        {
            this->direction = direction;
            this->direction.Normalize();
        }
        //射出方向を取得。
        const Vector3& GetDirection() const
        {
            return direction;
        }

        //座標を設定。
        void SetPosition(const Vector3& position)
        {
            this->position = position;
        }
        void SetPosition(float x, float y, float z)
        {
            SetPosition({ x,y,z });
        }

        //ライトのカラーを設定。
        void SetColor(const Vector3& color)
        {
            this->color = color;
        }
        void SetColor(float r, float g, float b)
        {
            SetColor({ r,g,b });
        }

        //スポットライトの影響範囲を設定。
        void SetRange(float range)
        {
            this->range = range;
        }

        //距離による影響率に累乗する値を設定
        void SetRangeAffectPow(float powParam)
        {
            pow.x = powParam;
        }


        //角度による影響率に累乗する値を設定
        void SetAngleAffectPow(float powParam)
        {
            pow.y = powParam;
        }

        //光の射出角度を設定。
        void SetAngle(float angle)
        {
            this->angle = angle;
        }
        //光の射出角度を取得。
        float GetAngle() const
        {
            return angle;
        }

        //座標を取得。
        const Vector3& GetPosition() const
        {
            return position;
        }

        //影響範囲を取得。
        float GetRange() const
        {
            return range;
        }

        //ポイントライトを使用中にする。
        //エンジン内で使う用の関数なのでゲーム側から使用しないでください。
        void Use()
        {
            isUse = true;
        }

        //ポイントライトを未使用にする。
        //エンジン内で使う用の関数なのでゲーム側から使用しないでください。
        void UnUse()
        {
            isUse = false;
        }

        //更新。
        //エンジン内で使う用の関数なのでゲーム側から使用しないでください。
        void Update();
    };

    //ライト構造体
    struct Light
    {
        DirectionalLight directionalLight[MAX_DIRECTIONAL_LIGHT];   // ディレクショナルライトの配列。
        SPointLight pointLights[MAX_POINT_LIGHT];                   // ポイントライトの配列。
        SSpotLight spotLights[MAX_SPOT_LIGHT];                      // スポットライトの配列
        Matrix mViewProjInv;    // ビュープロジェクション行列の逆行列
        Vector3 eyePos;         // カメラの位置
        int numPointLight;      // ポイントライトの数
        Vector3 ambientLight;   // 環境光
        int numSpotLight;       // スポットライトの数
    };

    //シーンライトクラス。
    class SceneLight : public Noncopyable
    {
    public:
        SceneLight();
        //初期化
        void Init();

        //シーンライトを取得
        Light& GetSceneLight()
        {
            return m_light;
        }

        //ディレクションライトのパラメータを設定
        void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
        {
            m_light.directionalLight[lightNo].direction = direction;
            m_light.directionalLight[lightNo].color = color;
        }

        //影をキャストする？
        bool IsCastShadow(int ligNo)
        {
            return m_light.directionalLight[ligNo].castShadow;
        }

        //シーンにポイントライトを追加
        SPointLight* NewPointLight()
        {
            return NewDynamicLight<SPointLight>(m_unusePointLightQueue);
        }
        //シーンからポイントライトを削除
        void DeletePointLight(SPointLight* pointLight)
        {
            DeleteDynamicLight<SPointLight>(pointLight, m_unusePointLightQueue);
        }

        //シーンにスポットライトを追加
        SSpotLight* NewSpotLight()
        {
            return NewDynamicLight<SSpotLight>(m_unuseSpotLightQueue);
        }
        //シーンからスポットライトを削除
        void DeleteSpotLight(SSpotLight* spotLight)
        {
            DeleteDynamicLight<SSpotLight>(spotLight, m_unuseSpotLightQueue);
        }

        //環境光を設定
        void SetAmbient(Vector3 ambient)
        {
            m_light.ambientLight = ambient;
        }
        
        //更新
        void Update();

    private:
        //新しい動的ライトを追加
        template<class TDynamicLight>
        TDynamicLight* NewDynamicLight(std::deque<TDynamicLight*>& que)
        {
            if (que.empty()) {
                // これ以上ライトを追加することはできない。
                return nullptr;
            }
            // 未使用のライトをでキューから取り出す。
            TDynamicLight* newPt = que.front();
            // 使用中にする。
            newPt->Use();
            // 取り出した要素を先頭から除去。
            que.pop_front();
            return newPt;
        }
        //シーンから動的ライトを削除
        template<class TDynamicLight>
        void DeleteDynamicLight(TDynamicLight* light, std::deque< TDynamicLight*>& que)
        {
            if (light != nullptr) {
                // フラグを未使用に変更する。
                light->UnUse();
                // 未使用リストに追加する。
                que.push_back(light);
            }
        }

    private:
        Light m_light;  //シーンライト
        std::deque<SPointLight*> m_unusePointLightQueue;    //未使用のポイントライトのキュー。
        std::deque<SSpotLight*> m_unuseSpotLightQueue;      //未使用のスポットライトのキュー。

    };
}

