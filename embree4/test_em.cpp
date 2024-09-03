#include <iostream>
#include <cstdlib>
#include "rtcore.h"

int main(){
    RTCDevice device = rtcNewDevice(nullptr);
    if (!device) {
        std::cerr << "Failed to create Embree device." << std::endl;
        exit(1);
    }
    RTCScene scene = rtcNewScene(device);
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    // 頂点データを設定
    float* vertices = (float*)rtcSetNewGeometryBuffer(
    geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, sizeof(float) * 3, 3);

    // 頂点データの設定 (例)
    vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f; // 1つ目の頂点
    vertices[3] = 1.0f; vertices[4] = 0.0f; vertices[5] = 0.0f; // 1つ目の頂点
    vertices[6] = 0.0f; vertices[7] = 1.0f; vertices[8] = 0.0f; // 1つ目の頂点
    // ... 他の頂点も同様に設定

    // インデックスデータを設定
    unsigned int* indices = (unsigned int*)rtcSetNewGeometryBuffer(
    geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, sizeof(unsigned int) * 3, 1);

    // インデックスデータの設定 (例)
    for(int i = 0; i < 9; i++){
        indices[i] = i;
    }

    // 法線や他の頂点属性が必要な場合、それらのバッファも設定

    rtcAttachGeometry(scene, geom);
    rtcReleaseGeometry(geom);  // シーンに追加した後は解放可能

    rtcCommitScene(scene);

    // レイの設定
RTCRayHit ray;
ray.ray.org_x = 1.0f;
ray.ray.org_y = 1.0f;
ray.ray.org_z = 1.0f;
ray.ray.dir_x = 0.0f;
ray.ray.dir_y = 0.0f;
ray.ray.dir_z = -1.0f;
ray.ray.tnear = 0.0f;
ray.ray.tfar = std::numeric_limits<float>::infinity();
ray.hit.geomID = RTC_INVALID_GEOMETRY_ID;
ray.hit.primID = RTC_INVALID_GEOMETRY_ID;

// レイトレーシングテスト
RTCIntersectContext context;
rtcInitIntersectContext(&context);
rtcIntersect1(scene, &context, &ray);

// 結果の確認
if (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
    std::cout << "Intersection found at t = " << ray.ray.tfar << std::endl;
}

return 0;
}