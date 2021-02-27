 //---------------------------------------------------------------------------
//!	@file   gpu_PMD.cpp
//!	@brief	GPU PMD
//---------------------------------------------------------------------------
#include "DirectXTex/DirectXTex/DirectXTex.h"

#include<map>
#include <sstream>

#include"gpu_PMDActor.h"

namespace gpu {

//-----------------------------------------------------------------------
//! PMD情報
//-----------------------------------------------------------------------
struct Desc
{
    std::string                 strModelPath_;   //!< モデルパス
    size_t                      verticesSize_;   //!< 1頂点当たりのバイト数
    std::vector<unsigned char>  vertices_;       //!< 頂点バッファ確保
    std::vector<unsigned short> indices_;        //!< インデックスバッファ確保
    unsigned int                materialNum_;    //!< マテリアル数
    std::vector<Material>       materials_;      //!< 転送用マテリアル
    std::vector<PMDMaterial>    pmdMaterials_;   //!< PMDマテリアル
    std::vector<Bone>           bones_;          //!< ボーン
};

//===========================================================================
//! PMD実装部
//===========================================================================
class PMDImpl final : public gpu::PMD
{
public:

    //! コンストラクタ
    PMDImpl(const Desc& desc, const std::vector<PMDIK> ikData)
    {
        desc_            = desc;
        ikData_          = ikData;

        transform_.world = XMMatrixIdentity();
        angle_           = 0.0f;
        duration_        = 0;
        startTime_ = timeGetTime();
        
        CreateTextureLoaderTable();     // 拡張子に対応したLoadFrom「画像形式」Fileでテクスチャ読み込み
        CreateVertexBufferView();       // 頂点バッファビューの作成
        CreateIndexBufferView();        // インデックスバッファの作成
        MaterialLoadTextureFromFile();  // マテリアルごとのテクスチャ読み込み
        BoneNodeTable();                // ボーン名とボーンノードを紐づけ
        CreateBoneMatrixBufferView();   // ボーン定数バッファビュー作成
        CreateMaterialBuffer();         // マテリアルバッファの作成
        CreateShaderResourceView();     // シェーダーリソースビュー読み込み

        // テスト
        //LoadVMDFile("../../../directx12_resource/motion/pose.vmd");
        //{
        //    auto  armnode = boneNodeTable_["左腕"];
        //    auto& armpos  = armnode.startPos;
        //    auto  armmat  = XMMatrixTranslation(-armpos.x, -armpos.y, -armpos.z) * XMMatrixRotationZ(XM_PIDIV2) * XMMatrixTranslation(armpos.x, armpos.y, armpos.z);
        //    auto  elbownode = boneNodeTable_["左ひじ"];
        //    auto& elbowpos  = elbownode.startPos;
        //    auto  elbowmat  = XMMatrixTranslation(-elbowpos.x, -elbowpos.y, -elbowpos.z) * XMMatrixRotationZ(-XM_PIDIV2) * XMMatrixTranslation(elbowpos.x, elbowpos.y, elbowpos.z);
        //    boneMatrices_[armnode.boneIdx]   = armmat;
        //    boneMatrices_[elbownode.boneIdx] = elbowmat;
        //    RecursiveMatrixMultipy(&boneNodeTable_["センター"], XMMatrixIdentity());
        //    std::copy(boneMatrices_.begin(), boneMatrices_.end(), mappedMatrices_ + 1);
        //}
    }

    //! デストラクタ
    virtual ~PMDImpl()
    {
    }

    // 初期化
    bool initialize()
    {
        return true;
    }

    //! VMDファイル読み込み
    virtual void LoadVMDFile(const char* filepath) override;

    //! モーション更新
    virtual void MotionUpdate() override;

    //! pmd描画
    virtual void PMDRender(ID3D12GraphicsCommandList* c) override;

private:

    // コピー禁止/move禁止
    PMDImpl(const PMDImpl&) = delete;
    PMDImpl(PMDImpl&&)      = delete;
    PMDImpl& operator=(const PMDImpl&) = delete;
    PMDImpl& operator=(PMDImpl&&) = delete;

private:

    //! 拡張子に対応したLoadFrom「画像形式」Fileでテクスチャ読み込み
    void CreateTextureLoaderTable();
    
    //! 頂点バッファビューの作成 
    void CreateVertexBufferView();

    //!　インデックスバッファビューの作成
    void CreateIndexBufferView();

    //! マテリアルごとにテクスチャ読み込み
    void MaterialLoadTextureFromFile();

    //! ボーン名とボーンノードを紐づけ
    void BoneNodeTable();

    //! ボーン行列バッファビューの作成
    void CreateBoneMatrixBufferView();

    //! マテリアルバッファの作成
    void CreateMaterialBuffer();

    //! シェーダーリソースビュー作成
    void CreateShaderResourceView();



    //!
    void RecursiveMatrixMultipy(BoneNode* node,const DirectX::XMMATRIX& mat, bool flg = false);

    //! ベジェ曲線補間
    float GetYFromXOnBezier(float x, const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b, uint8_t n = 12);

    //! CCD-IKによりボーン方向を解決
    void SolveCCDIK(const PMDIK& ik);

    //! 余弦定理IKによりボーン方向を解決
    void SolveCosineIK(const PMDIK& ik);

    //! LookAt行列によりボーン方向を解決
    void SolveLookAt(const PMDIK& ik);

    void IKSolve(int frameNo);

    //! 指定テクスチャのロード
    com_ptr<ID3D12Resource> LoadTextureFromFile(std::string& texPath);   

private:

    //----------------------------------------------------------
    //! @name マテリアル関連
    //----------------------------------------------------------
    //@{

    com_ptr<ID3D12Resource>       materialBuffer_;     //!< マテリアルバッファ
    u64                           materialBuffSize_;   //!< マテリアルバッファサイズ
    com_ptr<ID3D12DescriptorHeap> materialDescHeap_;   //!< マテリアルディスクリプタヒープ

    //@}

    //----------------------------------------------------------
    //! @name テクスチャ関連
    //----------------------------------------------------------
    //@{

    std::vector<com_ptr<ID3D12Resource>> textureResources_;   //!< テクスチャリソース
    std::vector<com_ptr<ID3D12Resource>> sphResources_;       //!< 乗算スフィアリソース
    std::vector<com_ptr<ID3D12Resource>> spaResources_;       //!< 加算スフィアリソース
    std::vector<com_ptr<ID3D12Resource>> toonResources_;      //!< トゥーンリソース

    using LoadLambda_t = std::function<HRESULT(const std::wstring& path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;   // ロード用のラムダ式を格納するための型(引数、戻り値)を定義
    //std::map< 拡張子, ラムダ式 >
    std::map<std::string, LoadLambda_t> loadLambdaTable_;   // 拡張子によって関数を呼び分けるためにラムダ式と拡張子文字列のマップを作る

    //@}

    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    Desc               desc_ = {};

    //@}
   
    //----------------------------------------------------------
    //! @name アプリケーション向けリソース
    //----------------------------------------------------------
    //@{
    
    com_ptr<ID3D12Resource>  vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;   //!< 頂点バッファView(VBV)
    com_ptr<ID3D12Resource>  indexBuffer;
    D3D12_INDEX_BUFFER_VIEW  indexBufferView_;    //!< インデックスバッファView(IBV)
    
    //@}

    //----------------------------------------------------------
    //! @name ボーン関連
    //----------------------------------------------------------
    //@{

    std::vector<DirectX::XMMATRIX> boneMatrices_;        //!< ボーンマトリックス
    com_ptr<ID3D12DescriptorHeap> boneMatricesDescHeap_; //!< ボーン行列ディスクリプタヒープ
    com_ptr<ID3D12Resource>       boneMatricesBuff_;     //!< ボーン行列バッファ

    std::unordered_map<std::string, BoneNode> boneNodeTable_;          //!< ボーンノードマップ
    std::vector<std::string>        boneNameArray_;          //!< インデックスから名前を検索しやすいようにしておく
    std::vector<BoneNode*>          boneNodeAddressArray_;   //!< インデックスからノードを検索しやすいようにしておく

    struct Transform
    {
        //内部に持ってるXMMATRIXメンバが16バイトアライメントであるため
        //Transformをnewする際には16バイト境界に確保する
        void*             operator new(size_t size);
        DirectX::XMMATRIX world;
    };

    Transform              transform_;
    DirectX::XMMATRIX*     mappedMatrices_;

    std::vector<PMDIK> ikData_;

    //IKオンオフデータ
    struct VMDIKEnable
    {
        uint32_t                              frameNo;
        std::unordered_map<std::string, bool> ikEnableTable;
    };
    
    std::vector<VMDIKEnable> ikEnableData_;

    std::vector<u32> kneeIdxes_;

    //@}

    //----------------------------------------------------------
    //! @name モーション関連
    //----------------------------------------------------------
    //@{

    struct KeyFrame
    {
        unsigned int frameNo;//アニメーション開始からのフレーム数
        DirectX::XMVECTOR quaternion;//クォータニオン
        DirectX::XMFLOAT3 offset;       //IKの初期座標からのオフセット情報
        DirectX::XMFLOAT2 p1, p2;       //ベジェの中間コントロールポイント
        KeyFrame(unsigned int fno, XMVECTOR& q, DirectX::XMFLOAT3& ofst, DirectX::XMFLOAT2& ip1, const DirectX::XMFLOAT2& ip2)
        : frameNo(fno),
          quaternion(q), 
          offset(ofst),
          p1(ip1),
          p2(ip2){}
    };

    std::unordered_map<std::string, std::vector<KeyFrame>> motiondata_;

    DWORD startTime_;   //アニメーション開始時点のミリ秒時刻
    u32   duration_;

    float angle_;   //テスト用Y軸回転
    
    //@}

};

//---------------------------------------------------------------------------
//! PMDをファイルから作成
//---------------------------------------------------------------------------
std::shared_ptr<gpu::PMD> createPMDFromFile(std::string path)
{
    std::string strModelPath = path;
    auto        fp           = fopen(strModelPath.c_str(), "rb");
    
    if(fp == nullptr) {
        //エラー処理
        assert(0);
    }

    // 先頭３バイトのシグネチャを考慮
    char signature[3];
    std::fread(signature, sizeof(signature), 1, fp);   // 先頭3バイトのシグネチャを考慮

    // モデルの基本情報を読み込み
    PMDHeader pmdheader = {};
    std::fread(&pmdheader, sizeof(pmdheader), 1, fp);   // モデルの基本情報読み込み

    // 頂点数読み込み
    u32 vertNum;                           // 頂点数
    std::fread(&vertNum, sizeof(vertNum), 1, fp);   // 頂点数読み込み

    // 頂点データ読み込み
    constexpr u32 vertexsize = 38;                                // 1頂点当たりのバイト数
    std::vector<u8>  vertices(vertNum * vertexsize);       // 頂点バッファ確保
    std::fread(vertices.data(), vertices.size(), 1, fp);             // 頂点データ読み込み

    // インデックス数読み込み
    u32 indicesNum;                                        // インデックス数
    std::fread(&indicesNum, sizeof(indicesNum), 1, fp);   // インデックス数を読み込み

    // インデックスデータ読み込み
    std::vector<u16> indices(indicesNum);                                               // vector型の変数indicesのサイズに反映(インデックス数分確保)
    std::fread(indices.data(), indices.size() * sizeof(indices[0]), 1, fp);   // インデックスデータ読み込み

    // マテリアル数読み込み
    u32 materialNum;                                         // マテリアル数
    std::fread(&materialNum, sizeof(materialNum), 1, fp);   // マテリアル数読み込み

    // マテリアルデータ読み込み
    std::vector<PMDMaterial> pmdMaterials(materialNum);
    std::fread(pmdMaterials.data(), pmdMaterials.size() * sizeof(PMDMaterial), 1, fp);   // マテリアルデータ読み込み

    // ボーン数の読み込み
    u16 boneNum = 0;
    std::fread(&boneNum, sizeof(boneNum), 1, fp);

    // ボーンの読み込み
    std::vector<Bone> bones_(boneNum);
    std::fread(bones_.data(), sizeof(Bone), boneNum, fp);

    // インバースキネマティックス数の読み込み
    u16 ikNum = 0;
    fread(&ikNum, sizeof(ikNum), 1, fp);

    std::vector<PMDIK> ikData(ikNum);
    for(auto& ik : ikData) {
       
        fread(&ik.boneIdx, sizeof(ik.boneIdx), 1, fp);
        fread(&ik.targetIdx, sizeof(ik.targetIdx), 1, fp);
       
        u8 chainLen = 0;
        fread(&chainLen, sizeof(chainLen), 1, fp);
        ik.nodeIdxes.resize(chainLen);
        
        fread(&ik.iterations, sizeof(ik.iterations), 1, fp);
        fread(&ik.limit, sizeof(ik.limit), 1, fp);
        
        if(chainLen == 0)
            continue;   //間ノード数が0ならばここで終わり
        
        fread(ik.nodeIdxes.data(), sizeof(ik.nodeIdxes[0]), chainLen, fp);
    }

    // PMDファイルクローズ
    std::fclose(fp);

    // 転送用データ作成
    std::vector<Material> materials;
    materials.resize(materialNum);
    {
        // コピー
        for(int i = 0; i < pmdMaterials.size(); ++i) {
            materials[i].indicesNum           = pmdMaterials[i].indicesNum;
            materials[i].material.diffuse     = pmdMaterials[i].diffuse;
            materials[i].material.alpha       = pmdMaterials[i].alpha;
            materials[i].material.specular    = pmdMaterials[i].specular;
            materials[i].material.specularity = pmdMaterials[i].specularity;
            materials[i].material.ambient     = pmdMaterials[i].ambient;
            materials[i].additional.toonIndex = pmdMaterials[i].toonIdx;
        }
    }

    // パラメーターの保存
    Desc desc{};
    desc.strModelPath_ = strModelPath;
    desc.verticesSize_ = vertexsize;
    desc.vertices_ = vertices;
    desc.indices_  = indices;
    desc.materialNum_ = materialNum;
    desc.materials_   = materials;
    desc.pmdMaterials_ = pmdMaterials;
    desc.bones_        = bones_;

    //---------------------------------------------------------------------
    // PMDの初期化
    //---------------------------------------------------------------------
    auto p = std::make_shared<gpu::PMDImpl>(desc, ikData);

    if(!p->initialize()) {
        p.reset();
    }
    return p;
}

//---------------------------------------------------------------------------
//! VMDファイル読み込み 
//! @param  filepath ファイルパス
//---------------------------------------------------------------------------
void PMDImpl::LoadVMDFile(const char* filepath)
{
    struct VMDKeyFrame
    {
        char          boneName[15];   //ボーン名
        unsigned int  frameNo;        //フレーム番号
        XMFLOAT3      location;       //位置
        XMFLOAT4      quaternion;     //クォータニオン(回転)
        unsigned char bezier[64];     //[4][4][4]ベジェ補間パラメータ
    };

#pragma pack(1)
    //表情データ(頂点モーフデータ)
    struct VMDMorph
    {
        char  name[15];   //名前(パディングしてしまう)
        u32   frameNo;    //フレーム番号
        float weight;     //ウェイト(0.0f～1.0f)
    };                    //全部で23バイトなのでpragmapackで読む
#pragma pack()

#pragma pack(1)
    //カメラ
    struct VMDCamera
    {
        u32      frameNo;             // フレーム番号
        float    distance;            // 距離
        XMFLOAT3 pos;                 // 座標
        XMFLOAT3 eulerAngle;          // オイラー角
        u8       Interpolation[24];   // 補完
        u32      fov;                 // 視界角
        u8       persFlg;             // パースフラグON/OFF
    };                                //61バイト(これもpragma pack(1)の必要あり)
#pragma pack()

    // ライト照明データ
    struct VMDLight
    {
        uint32_t frameNo;   // フレーム番号
        XMFLOAT3 rgb;       //ライト色
        XMFLOAT3 vec;       //光線ベクトル(平行光線)
    };

#pragma pack(1)
    // セルフ影データ
    struct VMDSelfShadow
    {
        uint32_t frameNo;    // フレーム番号
        uint8_t  mode;       //影モード(0:影なし、1:モード１、2:モード２)
        float    distance;   //距離
    };
#pragma pack()

    auto fp = fopen(filepath,"rb");
    fseek(fp, 50, SEEK_SET);// 最初の50バイトをとばす

    // モーションデータ数
    unsigned int keyframeNum = 0;
    fread(&keyframeNum, sizeof(keyframeNum), 1, fp);

    std::vector<VMDKeyFrame> keyframes(keyframeNum);
    for(auto& keyframe : keyframes) {
        fread(keyframe.boneName, sizeof(keyframe.boneName), 1, fp);   //ボーン名
        fread(&keyframe.frameNo, 
            sizeof(keyframe.frameNo)              //フレーム番号
                  + sizeof(keyframe.location)     //位置（IKの時に使用）
                  + sizeof(keyframe.quaternion)   //クォータニオン
                  + sizeof(keyframe.bezier),      // 補間ベジェデータ
            1, fp);
    }

    uint32_t morphCount = 0;
    fread(&morphCount, sizeof(morphCount), 1, fp);

    std::vector<VMDMorph> morphs(morphCount);
    fread(morphs.data(), sizeof(VMDMorph), morphCount, fp);
    
    // vmdカメラ数
    uint32_t vmdCameraCount = 0;
    fread(&vmdCameraCount, sizeof(vmdCameraCount), 1, fp);

    // vmdカメラデータ
    std::vector<VMDCamera> cameraData(vmdCameraCount);
    fread(cameraData.data(), sizeof(VMDCamera), vmdCameraCount, fp);

    // vmdライト数
    uint32_t vmdLightCount = 0;
    fread(&vmdLightCount, sizeof(vmdLightCount), 1, fp);

    // vmdライトデータ
    std::vector<VMDLight> lights(vmdLightCount);
    fread(lights.data(), sizeof(VMDLight), vmdLightCount, fp);

    // セルフシャドー数
    uint32_t selfShadowCount = 0;
    fread(&selfShadowCount, sizeof(selfShadowCount), 1, fp);

    // セルフシャドーデータ
    std::vector<VMDSelfShadow> selfShadowData(selfShadowCount);
    fread(selfShadowData.data(), sizeof(VMDSelfShadow), selfShadowCount, fp);

    //IKオンオフ切り替わり数
    uint32_t ikSwitchCount = 0;
    fread(&ikSwitchCount, sizeof(ikSwitchCount), 1, fp);
    //IK切り替えのデータ構造は少しだけ特殊で、いくつ切り替えようが
    //そのキーフレームは消費されます。その中で切り替える可能性のある
    //IKの名前とそのフラグがすべて登録されている状態です。

    //ここからは気を遣って読み込みます。キーフレームごとのデータであり
    //IKボーン(名前で検索)ごとにオン、オフフラグを持っているというデータであるとして
    //構造体を作っていきましょう。
    ikEnableData_.resize(ikSwitchCount);
    for(auto& ikEnable : ikEnableData_) {
        //キーフレーム情報なのでまずはフレーム番号読み込み
        fread(&ikEnable.frameNo, sizeof(ikEnable.frameNo), 1, fp);
        
        //次に可視フラグがありますがこれは使用しないので1バイトシークでも構いません
        uint8_t visibleFlg = 0;
        fread(&visibleFlg, sizeof(visibleFlg), 1, fp);
        
        //対象ボーン数読み込み
        uint32_t ikBoneCount = 0;
        fread(&ikBoneCount, sizeof(ikBoneCount), 1, fp);
        
        //ループしつつ名前とON/OFF情報を取得
        for(int i = 0; i < ikBoneCount; ++i) {
            char ikBoneName[20];
            fread(ikBoneName, _countof(ikBoneName), 1, fp);
            
            uint8_t flg = 0;
            fread(&flg, sizeof(flg), 1, fp);
            
            ikEnable.ikEnableTable[ikBoneName] = flg;
        }
    }

    fclose(fp);

    //VMDのキーフレームデータから、実際に使用するキーフレームテーブルへ変換
    for(auto& f : keyframes) {
        motiondata_[f.boneName].emplace_back(KeyFrame(f.frameNo, XMLoadFloat4(&f.quaternion), f.location,
                                                      XMFLOAT2((float)f.bezier[3] / 127.0f, (float)f.bezier[7] / 127.0f),
                                                      XMFLOAT2((float)f.bezier[11] / 127.0f, (float)f.bezier[15] / 127.0f)));
        duration_= std::max<unsigned int>(duration_, f.frameNo);
    }

    for(auto& motion : motiondata_) {
        sort(motion.second.begin(), motion.second.end(),
             [](const KeyFrame& lval, const KeyFrame& rval) {
                 return lval.frameNo < rval.frameNo;
             });
    }

    for(auto& bonemotion : motiondata_) {
        auto itBoneNode = boneNodeTable_.find(bonemotion.first);
        if(itBoneNode == boneNodeTable_.end()) {
            continue;
        }
        auto& node = itBoneNode->second;
        auto& pos  = node.startPos;
        auto  mat  = XMMatrixTranslation(-pos.x, -pos.y, -pos.z) 
            * XMMatrixRotationQuaternion(bonemotion.second[0].quaternion)
            * XMMatrixTranslation(pos.x,pos.y,pos.z);

        boneMatrices_[node.boneIdx] = mat;
    }

    RecursiveMatrixMultipy(&boneNodeTable_["センター"], XMMatrixIdentity());
    std::copy(boneMatrices_.begin(), boneMatrices_.end(), mappedMatrices_ + 1);

}

//---------------------------------------------------------------------------
//! pmd描画
//! @param  c コマンドリスト
//---------------------------------------------------------------------------
void PMDImpl::PMDRender(ID3D12GraphicsCommandList* c)
{
    c->IASetVertexBuffers(0, 1, &vertexBufferView_);   // 頂点バッファビューを設定
    c->IASetIndexBuffer(&indexBufferView_);            // インデックスバッファビューを設定

    ID3D12DescriptorHeap* transformHeaps[] = { boneMatricesDescHeap_.Get() };
    c->SetDescriptorHeaps(1, transformHeaps);
    c->SetGraphicsRootDescriptorTable(1, boneMatricesDescHeap_.Get()->GetGPUDescriptorHandleForHeapStart());

    ID3D12DescriptorHeap* materialDescHeaps[] = { materialDescHeap_.Get() };
    c->SetDescriptorHeaps(1, materialDescHeaps);   //マテリアル

    // 描画命令を設定
    // ヒープの先頭
    auto materialHandle = materialDescHeap_.Get()->GetGPUDescriptorHandleForHeapStart();
    // 最初はオフセットなし
    unsigned int idxOffset = 0;
    // 定数バッファビューとシェーダーリソースビューのインクリメントサイズ
    auto cbvsrvIncsize = d3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 5;   // 5倍
    for(auto& m : desc_.materials_) {
        c->SetGraphicsRootDescriptorTable(2, materialHandle);
        //インデックス数  インスタンス数  すでに描画したインデックス数の合計
        c->DrawIndexedInstanced(m.indicesNum, 1, idxOffset, 0, 0);

        // 次のビューのために2倍進める
        materialHandle.ptr += cbvsrvIncsize;
        // オフセットを今回描画したインデックス数だけ増加
        idxOffset += m.indicesNum;
    }
}

//---------------------------------------------------------------------------
//! 拡張子に対応したLoadFrom「画像形式」Fileでテクスチャ読み込み
//---------------------------------------------------------------------------
void PMDImpl::CreateTextureLoaderTable()
{
    //-----------------------------------------------------------------------
    // 拡張子に対応したLoadFrom「画像形式」Fileでテクスチャ読み込み
    // 対応表作成
    //-----------------------------------------------------------------------
    
    // 「WIC」画像形式でテクスチャ読み込み
    loadLambdaTable_["sph"] = loadLambdaTable_["spa"] = loadLambdaTable_["bmp"] = loadLambdaTable_["png"] = loadLambdaTable_["jpg"] =
        [](const std::wstring& path, TexMetadata* meta, ScratchImage& img) -> HRESULT {
        return LoadFromWICFile(path.c_str(), 0, meta, img);
    };
    // 「WIC」画像形式でテクスチャ読み込み
    loadLambdaTable_["tga"] = [](const std::wstring& path, TexMetadata* meta, ScratchImage& img) -> HRESULT {
        return LoadFromTGAFile(path.c_str(), meta, img);
    };
    // 「DDS」画像形式でテクスチャ読み込み
    loadLambdaTable_["dds"] = [](const std::wstring& path, TexMetadata* meta, ScratchImage& img) -> HRESULT {
        return LoadFromDDSFile(path.c_str(), 0, meta, img);
    };
}

//---------------------------------------------------------------------------
//! 頂点バッファビューの作成
//---------------------------------------------------------------------------
void PMDImpl::CreateVertexBufferView()
{
    //-----------------------------------------------------------------------
    // 頂点バッファの作成
    //-----------------------------------------------------------------------
    {
        // ID3D12Dvice::CreateHeap()メソッドとID3D12Device::CreatePlacedResource()メソッドを
        // 使用して生成する方法もある
        // GPU側のメモリ領域を確保
        if(FAILED(d3dDevice()->CreateCommittedResource(
               &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
               D3D12_HEAP_FLAG_NONE,
               &CD3DX12_RESOURCE_DESC::Buffer(desc_.vertices_.size()),
               D3D12_RESOURCE_STATE_GENERIC_READ,
               nullptr,
               IID_PPV_ARGS(vertexBuffer.ReleaseAndGetAddressOf())))) {
            assert(0);
        }

        // 頂点情報をマップ
        unsigned char* vertMap = nullptr;
        // MAP設定
        if(FAILED(vertexBuffer->Map(0, nullptr, (void**)&vertMap))) {
            assert(0);
        }
        // コピー
        std::copy(desc_.vertices_.begin(), desc_.vertices_.end(), vertMap);
        // マップ解除
        vertexBuffer->Unmap(0, nullptr);
    }

    //----------------------------------------------------------
    // 頂点バッファビューの作成
    //----------------------------------------------------------
    vertexBufferView_.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes    = desc_.vertices_.size();
    vertexBufferView_.StrideInBytes  = desc_.verticesSize_;
}

//---------------------------------------------------------------------------
//! インデックスバッファビューの作成
//---------------------------------------------------------------------------
void PMDImpl::CreateIndexBufferView()
{
    //-----------------------------------------------------------------------
    // インデックスバッファの設定
    //-----------------------------------------------------------------------
    {
        // インデックスバッファを作成
        if(FAILED(d3dDevice()->CreateCommittedResource(
               &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
               D3D12_HEAP_FLAG_NONE,
               &CD3DX12_RESOURCE_DESC::Buffer(desc_.indices_.size() * sizeof(desc_.indices_[0])),
               D3D12_RESOURCE_STATE_GENERIC_READ,
               nullptr,
               IID_PPV_ARGS(indexBuffer.ReleaseAndGetAddressOf())))) {
            assert(0);
        }

        // インデックスバッファをマップ
        unsigned short* mappedIdx = nullptr;
        // MAP設定
        indexBuffer->Map(0, nullptr, (void**)&mappedIdx);
        // コピー
        std::copy(desc_.indices_.begin(), desc_.indices_.end(), mappedIdx);
        // マップ解除
        indexBuffer->Unmap(0, nullptr);
    }

    //----------------------------------------------------------
    // インデックスバッファビューの作成
    //----------------------------------------------------------
    indexBufferView_.BufferLocation = indexBuffer->GetGPUVirtualAddress();
    indexBufferView_.Format         = DXGI_FORMAT_R16_UINT;
    indexBufferView_.SizeInBytes    = desc_.indices_.size() * sizeof(desc_.indices_[0]);

}

//---------------------------------------------------------------------------
//! マテリアルごとにテクスチャ読み込み
//---------------------------------------------------------------------------
void PMDImpl::MaterialLoadTextureFromFile()
{
    //-----------------------------------------------------------------------
    // マテリアルごとにテクスチャ読み込み
    //-----------------------------------------------------------------------
    textureResources_.resize(desc_.materialNum_);
    sphResources_.resize(desc_.materialNum_);
    spaResources_.resize(desc_.materialNum_);
    toonResources_.resize(desc_.materialNum_);

    for(int i = 0; i < desc_.materials_.size(); ++i) {
        //-----------------------------------------------------------------------
        // トゥーンリソースの読み込み
        //-----------------------------------------------------------------------
        std::string toonFilePath = "../../../directx12_resource/Data/";
        char        toonFileName[32];
        sprintf(toonFileName, "toon/toon%02d.bmp", desc_.pmdMaterials_[i].toonIdx + 1);
        toonFilePath += toonFileName;
        toonResources_[i] = LoadTextureFromFile(toonFilePath);

        //-----------------------------------------------------------------------
        // テクスチャが読み込まれなかったらcontinue
        //-----------------------------------------------------------------------
        if(strlen(desc_.pmdMaterials_[i].texFilePath) == 0) {
            toonResources_[i] = nullptr;
            continue;
        }

        // ファイルパスを取得
        std::string texFileName = desc_.pmdMaterials_[i].texFilePath;
        std::string sphFileName = "";
        std::string spaFileName = "";

        // スプリッタがある(区切り文字あり)
        if(std::count(texFileName.begin(), texFileName.end(), '*') > 0) {
            // スプリッタファイル名を取得
            auto namepair = SplitFileName(texFileName);

            // firstが乗算スフィア
            if(GetExtension(namepair.first) == "sph") {
                texFileName = namepair.second;
                sphFileName = namepair.first;
            }
            // firstが加算スフィア
            if(GetExtension(namepair.first) == "spa") {
                texFileName = namepair.second;
                spaFileName = namepair.first;
            }

            // firstが乗算スフィアでも加算スフィアでもない時
            if(GetExtension(namepair.first) != "sph" && GetExtension(namepair.first) != "spa") {
                texFileName = namepair.first;

                // secondが乗算スフィア
                if(GetExtension(namepair.second) == "sph") {
                    sphFileName = namepair.second;
                }
                // secondが加算スフィア
                if(GetExtension(namepair.second) == "spa") {
                    spaFileName = namepair.second;
                }
            }
        }
        else {   // スプリッタがない(区切り文字なし)
            // 乗算スフィア
            if(GetExtension(desc_.pmdMaterials_[i].texFilePath) == "sph") {
                sphFileName = desc_.pmdMaterials_[i].texFilePath;
                texFileName = "";
            }
            // 加算スフィア
            if(GetExtension(desc_.pmdMaterials_[i].texFilePath) == "spa") {
                spaFileName = desc_.pmdMaterials_[i].texFilePath;
                texFileName = "";
            }
            // 乗算スフィアでも加算スフィアでもない場合
            if(GetExtension(desc_.pmdMaterials_[i].texFilePath) == "sph" && GetExtension(desc_.pmdMaterials_[i].texFilePath) == "spa") {
                texFileName = desc_.pmdMaterials_[i].texFilePath;
            }
        }

        //-----------------------------------------------------------------------
        //モデルとテクスチャパスからアプリケーションからのテクスチャパスを得る
        //-----------------------------------------------------------------------
        if(texFileName != "") {
            auto texFilePath = GetTexturePathFromModelAndTexPath(desc_.strModelPath_, texFileName.c_str());
            // テクスチャーをロード
            textureResources_[i] = LoadTextureFromFile(texFilePath);
        }
        if(sphFileName != "") {
            auto sphFilePath = GetTexturePathFromModelAndTexPath(desc_.strModelPath_, sphFileName.c_str());
            // テクスチャーをロード
            sphResources_[i] = LoadTextureFromFile(sphFilePath);
        }
        if(spaFileName != "") {
            auto spaFilePath = GetTexturePathFromModelAndTexPath(desc_.strModelPath_, spaFileName.c_str());
            // テクスチャーをロード
            spaResources_[i] = LoadTextureFromFile(spaFilePath);
        }
    }
}

//---------------------------------------------------------------------------
//! シェーダーリソースビュー作成
//---------------------------------------------------------------------------
void PMDImpl::CreateShaderResourceView()
{
    //-----------------------------------------------------------------------
    // マテリアル用のディスクリプタヒープを作成
    //-----------------------------------------------------------------------
    D3D12_DESCRIPTOR_HEAP_DESC materialDescHeapDesc = {};
    materialDescHeapDesc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    materialDescHeapDesc.NodeMask                   = 0;
    materialDescHeapDesc.NumDescriptors             = desc_.materials_.size() * 5;   // マテリアル数を指定
    materialDescHeapDesc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    // マテリアル用のディスクリプタヒープを作成
    if(FAILED(d3dDevice()->CreateDescriptorHeap(
           &materialDescHeapDesc,
           IID_PPV_ARGS(&materialDescHeap_)))) {
        assert(0);
    }

    //-----------------------------------------------------------------------
    // シェーダーリソースビュー作成
    //-----------------------------------------------------------------------
    {
        // マテリアル用の定数バッファビューの詳細設定
        D3D12_CONSTANT_BUFFER_VIEW_DESC materialCBVDesc = {};
        materialCBVDesc.BufferLocation                  = materialBuffer_->GetGPUVirtualAddress();   // バッファアドレス
        materialCBVDesc.SizeInBytes                     = materialBuffSize_;

        // シェーダーリソースビューの詳細設定
        // 通常テクスチャビュー
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;   //後述
        srvDesc.ViewDimension                   = D3D12_SRV_DIMENSION_TEXTURE2D;              //2Dテクスチャ
        srvDesc.Texture2D.MipLevels             = 1;

        // マテリアルディスクリプタヒープハンドルの作成
        CD3DX12_CPU_DESCRIPTOR_HANDLE matDescHeapH(materialDescHeap_->GetCPUDescriptorHandleForHeapStart());
        // インクリメントサイズ
        auto incSize = d3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        // テクスチャ作成
        auto whiteTex = CreateWhiteTexture();
        auto blackTex = CreateBlackTexture();
        auto gradTex  = CreateGrayGradationTexture();

        // シェーダーリソースビュー作成
        for(int i = 0; i < desc_.materials_.size(); ++i) {
            // マテリアル用の定数バッファビュー
            d3dDevice()->CreateConstantBufferView(&materialCBVDesc, matDescHeapH);

            // インクリメントサイズ分ポインタをずらす
            matDescHeapH.ptr += incSize;
            // マテリアル定数バッファビューのアドレスをマテリアバッファサイズ分ずらす
            materialCBVDesc.BufferLocation += materialBuffSize_;

            //-----------------------------------------------------------------------
            // テクスチャリソースビュー
            //-----------------------------------------------------------------------
            if(textureResources_[i] == nullptr) {
                // フォーマット指定
                srvDesc.Format = whiteTex->GetDesc().Format;
                // シェーダリソースビュー作成
                d3dDevice()->CreateShaderResourceView(whiteTex, &srvDesc, matDescHeapH);
            }
            else {
                // フォーマット指定
                srvDesc.Format = textureResources_[i]->GetDesc().Format;
                // シェーダリソースビュー作成
                d3dDevice()->CreateShaderResourceView(textureResources_[i].Get(), &srvDesc, matDescHeapH);
            }
            // インクリメントサイズ分ポインタをずらす
            matDescHeapH.ptr += incSize;

            //-----------------------------------------------------------------------
            // 乗算スフィアマップリソースビュー
            //-----------------------------------------------------------------------
            // 白テクスチャ
            if(sphResources_[i] == nullptr) {
                srvDesc.Format = whiteTex->GetDesc().Format;
                // シェーダリソースビュー作成
                d3dDevice()->CreateShaderResourceView(whiteTex, &srvDesc, matDescHeapH);
            }
            else {
                srvDesc.Format = sphResources_[i]->GetDesc().Format;
                // シェーダリソースビュー作成
                d3dDevice()->CreateShaderResourceView(sphResources_[i].Get(), &srvDesc, matDescHeapH);
            }
            // インクリメントサイズ分ポインタをずらす
            matDescHeapH.ptr += incSize;

            //-----------------------------------------------------------------------
            // 加算スフィアマップリソースビュー
            //-----------------------------------------------------------------------
            // 黒テクスチャ
            if(spaResources_[i] == nullptr) {
                srvDesc.Format = blackTex->GetDesc().Format;
                // シェーダリソースビュー作成
                d3dDevice()->CreateShaderResourceView(blackTex, &srvDesc, matDescHeapH);
            }
            else {
                srvDesc.Format = spaResources_[i]->GetDesc().Format;
                // シェーダリソースビュー作成
                d3dDevice()->CreateShaderResourceView(spaResources_[i].Get(), &srvDesc, matDescHeapH);
            }
            // インクリメントサイズ分ポインタをずらす
            matDescHeapH.ptr += incSize;

            //-----------------------------------------------------------------------
            // トゥーンテクスチャリソースビュー
            //-----------------------------------------------------------------------
            if(toonResources_[i] == nullptr) {
                srvDesc.Format = gradTex->GetDesc().Format;
                // シェーダリソースビュー作成
                d3dDevice()->CreateShaderResourceView(gradTex, &srvDesc, matDescHeapH);
            }
            else {
                srvDesc.Format = toonResources_[i]->GetDesc().Format;
                // シェーダリソースビュー作成
                d3dDevice()->CreateShaderResourceView(toonResources_[i].Get(), &srvDesc, matDescHeapH);
            }
            // インクリメントサイズ分ポインタをずらす
            matDescHeapH.ptr += incSize;
        }
    }
}

//---------------------------------------------------------------------------
//! ボーン名とボーンノードを紐づけ
//---------------------------------------------------------------------------
void PMDImpl::BoneNodeTable()
{
    boneNameArray_.resize(desc_.bones_.size());
    boneNodeAddressArray_.resize(desc_.bones_.size());
    //ボーン情報構築
    //インデックスと名前の対応関係構築のために後で使う
    //ボーンノードマップを作る
    kneeIdxes_.clear();
    for(int idx = 0; idx < desc_.bones_.size(); ++idx) {
        auto& pb          = desc_.bones_[idx];
        auto& node        = boneNodeTable_[pb.boneName];
        node.boneIdx      = idx;
        node.startPos     = pb.pos;
        node.boneType     = pb.type;
        node.parentBone   = pb.parentNo;
        node.ikParentBone = pb.ikBoneNo;
        //インデックス検索がしやすいように
        boneNameArray_[idx]        = pb.boneName;
        boneNodeAddressArray_[idx] = &node;
        std::string boneName            = pb.boneName;
        if(boneName.find("ひざ") != std::string::npos) {
            kneeIdxes_.emplace_back(idx);
        }
    }
    // 親子関係を構築
    for(auto& pb : desc_.bones_) {
        //　親インデックスをチェック(あり得ない番号なら飛ばす)
        if(pb.parentNo >= desc_.bones_.size()) {
            continue;
        }
        auto parentName = boneNameArray_[pb.parentNo];
        boneNodeTable_[parentName].children.emplace_back(&boneNodeTable_[pb.boneName]);
    }

    boneMatrices_.resize(desc_.bones_.size());

    // ボーンマテリアルをすべて初期化
    std::fill(boneMatrices_.begin(), boneMatrices_.end(), XMMatrixIdentity());

    //IKデバッグ用
    auto getNameFromIdx = [&](uint16_t idx) -> std::string {
        auto it = find_if(boneNodeTable_.begin(), boneNodeTable_.end(), [idx](const std::pair<std::string, BoneNode>& obj) { return obj.second.boneIdx == idx; });

        if(it != boneNodeTable_.end()) {
            return it->first;
        }
        else {
            return "";
        }
    };

    for(auto& ik : ikData_) {
        std::ostringstream oss;
        oss << "IKボーン番号=" << ik.boneIdx << ":" << getNameFromIdx(ik.boneIdx) << std::endl;
        for(auto& node : ik.nodeIdxes) {
            oss << "\tノードボーン=" << node << ":" << getNameFromIdx(node) << std::endl;
        }
        OutputDebugString(oss.str().c_str());
    }
}

//---------------------------------------------------------------------------
//! ボーン行列バッファビューの作成
//---------------------------------------------------------------------------
void PMDImpl::CreateBoneMatrixBufferView()
{
    // GPUバッファ作成
    auto buffSize = sizeof(XMMATRIX) * (1 + boneMatrices_.size());
    buffSize      = (buffSize + 0xff) & ~0xff;
    if(FAILED(d3dDevice()->CreateCommittedResource(
           &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
           D3D12_HEAP_FLAG_NONE,
           &CD3DX12_RESOURCE_DESC::Buffer(buffSize),
           D3D12_RESOURCE_STATE_GENERIC_READ,
           nullptr,
           IID_PPV_ARGS(boneMatricesBuff_.ReleaseAndGetAddressOf())))) 
    {
        assert(0);
    }

    // マップ
    if(FAILED(boneMatricesBuff_->Map(0, nullptr, (void**)&mappedMatrices_))) {
        assert(0);
    }

    // データコピー
    mappedMatrices_[0] = transform_.world;
    //memcpy(mappedMatrices_ + 1, &boneMatrices_, boneMatrices_.size());
    std::copy(boneMatrices_.begin(), boneMatrices_.end(), mappedMatrices_ + 1);

    // ビュー用のディスクリプタヒープを作成
    D3D12_DESCRIPTOR_HEAP_DESC boneMatricesDescHeapDesc = {};
    boneMatricesDescHeapDesc.NumDescriptors             = 1;
    boneMatricesDescHeapDesc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    boneMatricesDescHeapDesc.NodeMask                   = 0;
    boneMatricesDescHeapDesc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    if(FAILED(d3dDevice()->CreateDescriptorHeap(&boneMatricesDescHeapDesc, IID_PPV_ARGS(boneMatricesDescHeap_.ReleaseAndGetAddressOf())))) {
        assert(0);
    }

    // ビュー作成
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation                  = boneMatricesBuff_->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes                     = buffSize;
    d3dDevice()->CreateConstantBufferView(&cbvDesc, boneMatricesDescHeap_->GetCPUDescriptorHandleForHeapStart());
}

//-----------------------------------------------------------------------
//! マテリアルバッファ作成
//-----------------------------------------------------------------------
void PMDImpl::CreateMaterialBuffer()
{
    //-----------------------------------------------------------------------
    // マテリアルバッファの作成
    //-----------------------------------------------------------------------
    {
        materialBuffSize_ = sizeof(MaterialForHlsl);
        materialBuffSize_ = (materialBuffSize_ + 0xff) & ~0xff;   // アライメント256区切り

        if(FAILED(gpu::d3dDevice()->CreateCommittedResource(
               &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
               D3D12_HEAP_FLAG_NONE,
               &CD3DX12_RESOURCE_DESC::Buffer(materialBuffSize_ * desc_.materials_.size()),   // もったいないので改善の余地あり
               D3D12_RESOURCE_STATE_GENERIC_READ,
               nullptr,
               IID_PPV_ARGS(materialBuffer_.ReleaseAndGetAddressOf())))) {
            assert(0);
        }

        // マップ
        char* gpuBufferMemory = nullptr;
        if(FAILED(materialBuffer_->Map(0, nullptr, (void**)&gpuBufferMemory))) {
            assert(0);
        }

        for(auto& m : desc_.materials_) {
            *((MaterialForHlsl*)gpuBufferMemory) = m.material;   // データコピー
            gpuBufferMemory += materialBuffSize_;                // 次のアライメント位置まで進める(256の倍数)
        }

        materialBuffer_->Unmap(0, nullptr);
    }
}

//---------------------------------------------------------------------------
//! 
//---------------------------------------------------------------------------
void PMDImpl::RecursiveMatrixMultipy(BoneNode* node, const DirectX::XMMATRIX& mat, bool flg)
{
    boneMatrices_[node->boneIdx] *= mat;
    for(auto& cnode : node->children) {
        RecursiveMatrixMultipy(cnode, boneMatrices_[cnode->boneIdx]);
    }
}

//---------------------------------------------------------------------------
//! ベジェ曲線曲線補間
//---------------------------------------------------------------------------
float PMDImpl::GetYFromXOnBezier(float x, const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b, uint8_t n)
{
    if(a.x == a.y && b.x == b.y) {
        return x;
    }

    float t = x;
    const float k0 = 1 + 3 * a.x - 3 * b.x;
    const float k1 = 3 * b.x - 6 * a.x;
    const float k2 = 3 * a.x;

    // 誤差の範囲内かどうかに使用する定数
    constexpr float epsilon = 0.0005f;

    for(int i = 0; i < n; ++i) {
        auto ft = k0 * t * t * t + k1 * t * t + k2 * t - x;

        // もし結果が0に近い(誤差の範囲内)なら打ち切る
        if(ft <= epsilon && ft >= -epsilon) {
            break;
        }

        t -= ft / 2;
    }

    // 求めたいtはすでに求めているのでyを計算する
    auto r = 1 - t;
    return t * t * t + 3 * t * t * r * b.y + 3 * t * r * r * a.y;
}

//---------------------------------------------------------------------------
//! モーション更新
//---------------------------------------------------------------------------
void PMDImpl::MotionUpdate()
{
    angle_ += 0.001f;
    mappedMatrices_[0] = XMMatrixRotationY(angle_);

    auto         elapsedTime = timeGetTime() - startTime_;   //経過時間を測る
    unsigned int frameNo     = 30 * (elapsedTime / 1000.0f);

    if(frameNo > duration_) {
        startTime_ = timeGetTime();
        frameNo = 0;
    }

    //行列情報クリア(してないと前フレームのポーズが重ね掛けされてモデルが壊れる)
    std::fill(boneMatrices_.begin(), boneMatrices_.end(), XMMatrixIdentity());

    //モーションデータ更新
    for(auto& bonemotion : motiondata_) {
        auto& boneName   = bonemotion.first;
        auto  itBoneNode = boneNodeTable_.find(boneName);
        if(itBoneNode == boneNodeTable_.end()) {
            continue;
        }
        auto node = itBoneNode->second;

        //合致するものを探す
        auto keyframes = bonemotion.second;

        auto rit = find_if(keyframes.rbegin(), keyframes.rend(), [frameNo](const KeyFrame& keyframe) {
            return keyframe.frameNo <= frameNo;
        });

        if(rit == keyframes.rend())
            continue;   //合致するものがなければ飛ばす

        XMMATRIX rotation = XMMatrixIdentity();
        XMVECTOR offset = XMLoadFloat3(&rit->offset);
        auto     it = rit.base();
        if(it != keyframes.end()) {
            auto t = static_cast<float>(frameNo - rit->frameNo) /
                     static_cast<float>(it->frameNo - rit->frameNo);
            t = GetYFromXOnBezier(t, it->p1, it->p2, 12);

            rotation = XMMatrixRotationQuaternion(
                XMQuaternionSlerp(rit->quaternion, it->quaternion, t));
            offset = XMVectorLerp(offset, XMLoadFloat3(&it->offset), t);
        }
        else {
            rotation = XMMatrixRotationQuaternion(rit->quaternion);
        }

        auto& pos = node.startPos;
        auto  mat = XMMatrixTranslation(-pos.x, -pos.y, -pos.z) *   //原点に戻し
                   rotation *                                       //回転
                   XMMatrixTranslation(pos.x, pos.y, pos.z);        //元の座標に戻す
        boneMatrices_[node.boneIdx] = mat * XMMatrixTranslationFromVector(offset);
    }

    RecursiveMatrixMultipy(&boneNodeTable_["センター"], XMMatrixIdentity());

    //IKSolve(frameNo);

    std::copy(boneMatrices_.begin(), boneMatrices_.end(), mappedMatrices_ + 1);
}

//---------------------------------------------------------------------------
//! CCD-IKによりボーン方向を解決
//! @param ik 対象IKオブジェクト
//---------------------------------------------------------------------------
void PMDImpl::SolveCCDIK(const PMDIK& ik)
{
    //誤差の範囲内かどうかに使用する定数
    constexpr float epsilon = 0.0005f;
    
    //ターゲット
    auto targetBoneNode  = boneNodeAddressArray_[ik.boneIdx];
    auto targetOriginPos = XMLoadFloat3(&targetBoneNode->startPos);

    auto     parentMat = boneMatrices_[boneNodeAddressArray_[ik.boneIdx]->ikParentBone];
    XMVECTOR det;
    auto     invParentMat  = XMMatrixInverse(&det, parentMat);
    auto     targetNextPos = XMVector3Transform(targetOriginPos, boneMatrices_[ik.boneIdx] * invParentMat);

    //まずはIKの間にあるボーンの座標を入れておく(逆順注意)
    std::vector<XMVECTOR> bonePositions;
    //auto endPos = XMVector3Transform(
    //	XMLoadFloat3(&_boneNodeAddressArray[ik.targetIdx]->startPos),
    //	//_boneMatrices[ik.targetIdx]);
    //	XMMatrixIdentity());
    //末端ノード
    auto endPos = XMLoadFloat3(&boneNodeAddressArray_[ik.targetIdx]->startPos);
    //中間ノード(ルートを含む)
    for(auto& cidx : ik.nodeIdxes) {
        //bonePositions.emplace_back(XMVector3Transform(XMLoadFloat3(&_boneNodeAddressArray[cidx]->startPos),
        //_boneMatrices[cidx] ));
        bonePositions.push_back(XMLoadFloat3(&boneNodeAddressArray_[cidx]->startPos));
    }

    std::vector<XMMATRIX> mats(bonePositions.size());
    fill(mats.begin(), mats.end(), XMMatrixIdentity());
    //ちょっとよくわからないが、PMDエディタの6.8°が0.03になっており、これは180で割っただけの値である。
    //つまりこれをラジアンとして使用するにはXM_PIを乗算しなければならない…と思われる。
    auto ikLimit = ik.limit * XM_PI;
    //ikに設定されている試行回数だけ繰り返す
    for(int c = 0; c < ik.iterations; ++c) {
        //ターゲットと末端がほぼ一致したら抜ける
        if(XMVector3Length(XMVectorSubtract(endPos, targetNextPos)).m128_f32[0] <= epsilon) {
            break;
        }
        //それぞれのボーンを遡りながら角度制限に引っ掛からないように曲げていく
        for(int bidx = 0; bidx < bonePositions.size(); ++bidx) {
            const auto& pos = bonePositions[bidx];

            //まず現在のノードから末端までと、現在のノードからターゲットまでのベクトルを作る
            auto vecToEnd    = XMVectorSubtract(endPos, pos);
            auto vecToTarget = XMVectorSubtract(targetNextPos, pos);
            vecToEnd         = XMVector3Normalize(vecToEnd);
            vecToTarget      = XMVector3Normalize(vecToTarget);

            //ほぼ同じベクトルになってしまった場合は外積できないため次のボーンに引き渡す
            if(XMVector3Length(XMVectorSubtract(vecToEnd, vecToTarget)).m128_f32[0] <= epsilon) {
                continue;
            }
            //外積計算および角度計算
            auto  cross  = XMVector3Normalize(XMVector3Cross(vecToEnd, vecToTarget));
            float angle  = XMVector3AngleBetweenVectors(vecToEnd, vecToTarget).m128_f32[0];
            angle        = std::min(angle, ikLimit);                  //回転限界補正
            XMMATRIX rot = XMMatrixRotationAxis(cross, angle);   //回転行列
            //posを中心に回転
            auto mat = XMMatrixTranslationFromVector(-pos) *
                       rot *
                       XMMatrixTranslationFromVector(pos);
            mats[bidx] *= mat;   //回転行列を保持しておく(乗算で回転重ね掛けを作っておく)
            //対象となる点をすべて回転させる(現在の点から見て末端側を回転)
            for(auto idx = bidx - 1; idx >= 0; --idx) {   //自分を回転させる必要はない
                bonePositions[idx] = XMVector3Transform(bonePositions[idx], mat);
            }
            endPos = XMVector3Transform(endPos, mat);
            //もし正解に近くなってたらループを抜ける
            if(XMVector3Length(XMVectorSubtract(endPos, targetNextPos)).m128_f32[0] <= epsilon) {
                break;
            }
        }
    }
    int idx = 0;
    for(auto& cidx : ik.nodeIdxes) {
        boneMatrices_[cidx] = mats[idx];
        ++idx;
    }
    auto node = boneNodeAddressArray_[ik.nodeIdxes.back()];
    RecursiveMatrixMultipy(node, parentMat, true);
}

//---------------------------------------------------------------------------
//! 余弦定理IKによりボーン方向を解決
//! @param ik 対象IKオブジェクト
//---------------------------------------------------------------------------
void PMDImpl::SolveCosineIK(const PMDIK& ik)
{
    std::vector<XMVECTOR>     positions;   //IK構成点を保存
    std::array<float, 2> edgeLens;    //IKのそれぞれのボーン間の距離を保存

    //ターゲット(末端ボーンではなく、末端ボーンが近づく目標ボーンの座標を取得)
    auto& targetNode = boneNodeAddressArray_[ik.boneIdx];
    auto  targetPos  = XMVector3Transform(XMLoadFloat3(&targetNode->startPos), boneMatrices_[ik.boneIdx]);

    //IKチェーンが逆順なので、逆に並ぶようにしている
    //末端ボーン
    auto endNode = boneNodeAddressArray_[ik.targetIdx];
    positions.emplace_back(XMLoadFloat3(&endNode->startPos));
    //中間及びルートボーン
    for(auto& chainBoneIdx : ik.nodeIdxes) {
        auto boneNode = boneNodeAddressArray_[chainBoneIdx];
        positions.emplace_back(XMLoadFloat3(&boneNode->startPos));
    }
    //ちょっと分かりづらいと思ったので逆にしておきます。そうでもない人はそのまま
    //計算してもらって構わないです。
    reverse(positions.begin(), positions.end());

    //元の長さを測っておく
    edgeLens[0] = XMVector3Length(XMVectorSubtract(positions[1], positions[0])).m128_f32[0];
    edgeLens[1] = XMVector3Length(XMVectorSubtract(positions[2], positions[1])).m128_f32[0];

    //ルートボーン座標変換(逆順になっているため使用するインデックスに注意)
    positions[0] = XMVector3Transform(positions[0], boneMatrices_[ik.nodeIdxes[1]]);
    //真ん中はどうせ自動計算されるので計算しない
    //先端ボーン
    positions[2] = XMVector3Transform(positions[2], boneMatrices_[ik.boneIdx]);   //ホンマはik.targetIdxだが…！？

    //ルートから先端へのベクトルを作っておく
    auto  linearVec = XMVectorSubtract(positions[2], positions[0]);
    float A         = XMVector3Length(linearVec).m128_f32[0];
    float B         = edgeLens[0];
    float C         = edgeLens[1];

    linearVec = XMVector3Normalize(linearVec);

    //ルートから真ん中への角度計算
    float theta1 = acosf((A * A + B * B - C * C) / (2 * A * B));

    //真ん中からターゲットへの角度計算
    float theta2 = acosf((B * B + C * C - A * A) / (2 * B * C));

    //「軸」を求める
    //もし真ん中が「ひざ」であった場合には強制的にX軸とする。
    XMVECTOR axis;
    if(std::find(kneeIdxes_.begin(), kneeIdxes_.end(), ik.nodeIdxes[0]) == kneeIdxes_.end()) {
        auto vm = XMVector3Normalize(XMVectorSubtract(positions[2], positions[0]));
        auto vt = XMVector3Normalize(XMVectorSubtract(targetPos, positions[0]));
        axis    = XMVector3Cross(vt, vm);
    }
    else {
        auto right = XMFLOAT3(1, 0, 0);
        axis       = XMLoadFloat3(&right);
    }

    //注意点…IKチェーンは根っこに向かってから数えられるため1が根っこに近い
    auto mat1 = XMMatrixTranslationFromVector(-positions[0]);
    mat1 *= XMMatrixRotationAxis(axis, theta1);
    mat1 *= XMMatrixTranslationFromVector(positions[0]);

    auto mat2 = XMMatrixTranslationFromVector(-positions[1]);
    mat2 *= XMMatrixRotationAxis(axis, theta2 - XM_PI);
    mat2 *= XMMatrixTranslationFromVector(positions[1]);

    boneMatrices_[ik.nodeIdxes[1]] *= mat1;
    boneMatrices_[ik.nodeIdxes[0]] = mat2 * boneMatrices_[ik.nodeIdxes[1]];
    boneMatrices_[ik.targetIdx]    = boneMatrices_[ik.nodeIdxes[0]];   //直前の影響を受ける
    //_boneMatrices[ik.nodeIdxes[1]] = _boneMatrices[ik.boneIdx];
    //_boneMatrices[ik.nodeIdxes[0]] = _boneMatrices[ik.boneIdx];
    //_boneMatrices[ik.targetIdx] *= _boneMatrices[ik.boneIdx];
}

//---------------------------------------------------------------------------
//! LookAt行列によりボーン方向を解決
//! @param ik 対象IKオブジェクト
//---------------------------------------------------------------------------
void PMDImpl::SolveLookAt(const PMDIK& ik)
{
    //この関数に来た時点でノードはひとつしかなく、チェーンに入っているノード番号は
    //IKのルートノードのものなので、このルートノードからターゲットに向かうベクトルを考えればよい
    auto rootNode   = boneNodeAddressArray_[ik.nodeIdxes[0]];
    auto targetNode = boneNodeAddressArray_[ik.targetIdx];   //!?

    auto opos1 = XMLoadFloat3(&rootNode->startPos);
    auto tpos1 = XMLoadFloat3(&targetNode->startPos);

    auto opos2 = XMVector3Transform(opos1, boneMatrices_[ik.nodeIdxes[0]]);
    auto tpos2 = XMVector3Transform(tpos1, boneMatrices_[ik.boneIdx]);

    auto originVec = XMVectorSubtract(tpos1, opos1);
    auto targetVec = XMVectorSubtract(tpos2, opos2);

    originVec = XMVector3Normalize(originVec);
    targetVec = XMVector3Normalize(targetVec);

    XMMATRIX mat = XMMatrixTranslationFromVector(-opos2) *
                   LookAtMatrix(originVec, targetVec, XMFLOAT3(0, 1, 0), XMFLOAT3(1, 0, 0)) *
                   XMMatrixTranslationFromVector(opos2);

    //auto parent = _boneNodeAddressArray[ik.boneIdx]->parentBone;

    boneMatrices_[ik.nodeIdxes[0]] = mat;
    // _boneMatrices[ik.boneIdx] * _boneMatrices[parent];
    //_boneMatrices[ik.targetIdx] = _boneMatrices[parent];
}

//---------------------------------------------------------------------------
//! 
//! @param framNo
//---------------------------------------------------------------------------
void PMDImpl::IKSolve(int frameNo)
{
    //いつもの逆から検索
    auto it = find_if(ikEnableData_.rbegin(), ikEnableData_.rend(),
                      [frameNo](const VMDIKEnable& ikenable) {
                          return ikenable.frameNo <= frameNo;
                      });
    //まずはIKのターゲットボーンを動かす
    for(auto& ik : ikData_) {   //IK解決のためのループ
        
        if(it != ikEnableData_.rend()) {
            auto ikEnableIt = it->ikEnableTable.find(boneNameArray_[ik.boneIdx]);
            if(ikEnableIt != it->ikEnableTable.end()) {
                if(!ikEnableIt->second) {   //もしOFFなら打ち切ります
                    continue;
                }
            }
        }
        
        auto childrenNodesCount = ik.nodeIdxes.size();
        switch(childrenNodesCount) {
            case 0:   //間のボーン数が0(ありえない)
                assert(0);
                continue;
            case 1:   //間のボーン数が1のときはLookAt
                SolveLookAt(ik);
                break;
            case 2:   //間のボーン数が2のときは余弦定理IK
                SolveCosineIK(ik);
                break;
            default:   //3以上の時はCCD-IK
                SolveCCDIK(ik);
        }
    }
}

//---------------------------------------------------------------------------
//! ロードテクスチャーファイル
//! @param  texPath テクスチャパス
//! @return         テクスチャバッファ 
//---------------------------------------------------------------------------
com_ptr<ID3D12Resource> PMDImpl::LoadTextureFromFile(std::string& texPath)
{
    //----------------------------------------------------------
    //! 既にロードされているかいないかを検索する
    //----------------------------------------------------------
    //std::map<ファイルパス,テクスチャバッファ>
    std::map<std::string, ID3D12Resource*> resourceTable;
    
    // マップの中からキーに対応するメソッドを取得
    // 取得出来なかった場合.end()を返す
    auto it = resourceTable.find(texPath);
    
    // メソッドを取得出来たら
    if(it != resourceTable.end()) {
        //テーブルに内にあったらロードするのではなくマップ内の
        //リソースを返す
        return resourceTable[texPath];
    }
    

    //----------------------------------------------------------
    //! 拡張子にあった関数でテクスチャ読み込み
    //----------------------------------------------------------
    TexMetadata  metadata   = {};
    ScratchImage scratchImg = {};
    {
        auto wtexpath = GetWideStringFromString(texPath);   //テクスチャのファイルパス
        auto ext      = GetExtension(texPath);              // 拡張子取得

        if(FAILED(loadLambdaTable_[ext](wtexpath, &metadata, scratchImg))) {
            return false;
        }
    }
    // 生データ抽出
    auto img = scratchImg.GetImage(0, 0, 0);

    //----------------------------------------------------------
    // テクスチャ-情報をGPUに転送
    //----------------------------------------------------------
    // テクスチャバッファ用のヒーププロパティ作成
    auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    // テクスチャバッファ用のリソース詳細設定
    auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        metadata.format,
        metadata.width,
        metadata.height,
        metadata.arraySize,
        metadata.mipLevels);

    // テクスチャバッファの作成
    com_ptr<ID3D12Resource> texbuff;
    if(FAILED(d3dDevice()->CreateCommittedResource(
           &texHeapProp,
           D3D12_HEAP_FLAG_NONE,   //特に指定なし
           &resDesc,
           D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
           nullptr,
           IID_PPV_ARGS(&texbuff)
        ))) {
        return false;
    }

    //----------------------------------------------------------
    // WriteToSubresourceの作成
    // テクスチャ―情報(texbuff)をGPUに転送
    //----------------------------------------------------------
    if(FAILED(texbuff->WriteToSubresource(
           0,
           nullptr,          // 全領域（書き込み領域の指定）
           img->pixels,      // 元データアドレス(書き込みたいデータのアドレス)
           img->rowPitch,    // 1ラインのサイズ(1行あたりのデータサイズ)
           img->slicePitch   // 全サイズ(スライスあたりのデータサイズ)
           ))) {
        return false;
    }

    resourceTable[texPath] = texbuff.Get();

    return texbuff;
}

//---------------------------------------------------------------------------
//! 
//---------------------------------------------------------------------------
void* PMDImpl::Transform::operator new(size_t size)
{
    return _aligned_malloc(size, 16);
}

}   // namespace gpu