//
// Created by Administrator on 2021/4/12.
//

#ifndef PUG_NEW_OFFSETS_H
#define PUG_NEW_OFFSETS_H

namespace Offset {
    // uint64 CurrentStates; 国际服状态
    static int dw_CameraCacheEntry;//CameraCacheEntry CameraCache;
    static int dw_MinimalViewInfo; //MinimalViewInfo POV;
    static int dw_Location;//class MinimalViewInfo  Vector Location;
    static int dw_Rotation; // class MinimalViewInfo Rotator Rotation;
    static int dw_FOV; //class MinimalViewInfo float FOV;
    static int GNames; //GNameEntryPoolAllocator + 0x14
    static int GWorld; //GWorld
    static int persistentLevelOffset; //Level* PersistentLevel;
    static int netDriverOffset; //NetDriver* NetDriver;
    static int serverConnectionOffset; //NetConnection* ServerConnection;
    static int playerControllerOffset; //PlayerController* PlayerController;
    static int uMyObjectOffset; //自身偏移 STExtraPlayerState* STExtraPlayerState;
    static int RotationAngleOffset;//旋转角
    static int PlayerControllerOffset;//STExtraPlayerController* STPlayerController;
    static int CoordinatePointerOffset;//对象坐标指针 SceneComponent* RootComponent;
    static int CoordinateOffset; //坐标偏移
    static int StateOffset;//人物状态 PawnStateRepSyncData PawnStateRepSyncData;
    static int TeamIDOffset;// Class: UAECharacter.Character.Pawn.Actor.Object 队伍 int TeamID;
    static int HealthOffset;//血量 float Health;
    static int BoneAddrOffset;//阵列  SkeletalMeshComponent* Mesh;
    static int BoneMatrixOffset; //基矩阵 0xcf0 SoftObjectPath MeshAssetPath;
    static int BoneOffset; //骨骼 //584 StaticMesh* StaticMesh;
    static int isBotOffset; //人机 bool bIsAI;
    static int PlayerNameOffset;//人物名字 FString PlayerName;
    static int BoxAddressOffset;//物品地址数组 PickUpItemData[] PickUpDataList;
    static int IDNumberOffset;//盒子物品数量
    static int vehicleCommonComponentOffset; //汽车配置指针 VehicleCommonComponent* VehicleCommon;
    static int vehicleHpOffset;//汽车血量 Class: VehicleCommonComponent.VehicleComponent.ActorComponent.Object float HP;
    static int vehicleFuelOffset; //汽车油量3 float Fuel;
    static int gunOffset;//枪偏移 射击武器实体 ShootWeaponEntity* ShootWeaponEntityComp;
    static int bulletVelocityOffset;//枪子弹速度 float BulletFireSpeed;
    static int resistanceOffset;//枪口上抬 float AccessoriesVRecoilFactor;
    static int fireButtonOffset; //开火键 bool bIsPressingFireBtn;
    static int adsOffset;//开镜 bool bIsGunADS;
    static int lastUpdateStatusKeyListOffset;//Cur 0x1bf0 AnimStatusKeyList LastUpdateStatusKeyList;
    static int equipWeaponOffset; //STExtraWeapon* EquipWeapon; 装备武器
    static int shootModeOffset; //byte ShootMode; 模式
    static int weaponOffset; //0x1BEC;//武器
    static int holdingStateOffset;// 0x1BDC;//手持武器状态
    //static int relativeScale3DOffset;//class: SceneComponent Vector RelativeScale3D;
    //static int boneLocationOffset;  //SoftObjectPath MeshAssetPath;
    static int perLODBiasTypeInfoOffset; //+ 骨骼30
    static int playerCameraManagerOffset; //PlayerCameraManager* PlayerCameraManager; 相机
    static int weaponIDOffset;//武器ID 654 5f4
    static int weaponAmmoOffset;//剩余子弹 int CurBulletNumInClip;
    static int weaponClipOffset;//弹夹 int CurMaxBulletNumInOneClip;
    static int vectorOffset;//向量
    static int bIsSkipCheckMeshRotateOffset;
    static int bDeadOffset;//判断死亡 bool bDead;
    static int STExtraPlayerStateOffset;//判断状态 STExtraPlayerState* STExtraPlayerState;
    static int CustomSpringArmComponentOffset;//头部 CustomSpringArmComponent* SpringArmComp;
    static int MaxDistFromMainCharOffset;//判断人 float MaxDistFromMainChar;
    static int LineOfSightToOffset;
    static int componentVelocityOffset;
    static int maxHealthOffset;
    static int UStructToSuperStruct;
    static int UObjectToClassPrivate;
    //Class: ULevel
    static int ULevelToAActors;
    static int ULevelToAActorsCount;
    static int PointerSize;
    //Class: FNameEntry
    static int FNameEntryToNameString;
    static int UObjectToInternalIndex;

    static int controlRotationOffset;

    static int pawnOffset;//class: Controller(can use Character)
    static int ClientSetLocationOffset;
    static int SetControlRotationOffset;
    static int AddPitchInputOffset;
    static int AddYawInputOfffset;

    static int pitch_off;
    static int yaw_off;
    static int line_off;

    static int GetGetActorBoundsOffset;
    static int GetBoneNameOffset;
    static int GetBoneLocationOffset;
    static int BoxItemSize;
    int PoseStateOffset;//[Offset: 0x1408, Size: 1]

    void chinaOffset32() {
        BoxItemSize = 48;
        UStructToSuperStruct = 0x20;
        UObjectToClassPrivate = 0xC;
        //Class: ULevel
        ULevelToAActors = 0x70;
        ULevelToAActorsCount = 0x74;
        PointerSize = 0x4;
        FNameEntryToNameString = 0x8 + 0x2;
        //Class: UObject
        UObjectToInternalIndex = 0x10;

        dw_CameraCacheEntry = 0x370;//CameraCacheEntry CameraCache;
        dw_MinimalViewInfo = 0x10; //MinimalViewInfo POV;
        dw_Location = 0x0;//class MinimalViewInfo  Vector Location;
        dw_Rotation = 0x18; // class MinimalViewInfo Rotator Rotation;
        dw_FOV = 0x24; //class MinimalViewInfo float FOV;

        GNames = 0x7A8AFDC; //GNameEntryPoolAllocator + 0x2c  0x14
        GWorld = 0x77A8A14; //GWorlds
        persistentLevelOffset = 0x20; //Level* PersistentLevel;
        netDriverOffset = 0x24; //NetDriver* NetDriver;
        serverConnectionOffset = 0x64; //NetConnection* ServerConnection;
        playerControllerOffset = 0x20; //PlayerController* PlayerController;
        uMyObjectOffset = 0x19c8;//自身0x19c8; 0x34c //自身偏移 STExtraPlayerState* STExtraPlayerState;
        RotationAngleOffset = 0xD0;//旋转角
        PlayerControllerOffset = 0x29fc;//STExtraPlayerController* STPlayerController;
        CoordinatePointerOffset = 0x170;//对象坐标指针 SceneComponent* RootComponent;
        CoordinateOffset = 0X160; //坐标偏移
        StateOffset = 0x9f8;//人物状态 PawnStateRepSyncData PawnStateRepSyncData;
        TeamIDOffset = 0x66c;// Class: UAECharacter.Character.Pawn.Actor.Object 队伍 int TeamID;
        HealthOffset = 0x894;//血量 float Health;
        BoneAddrOffset = 0x358;//阵列  SkeletalMeshComponent* Mesh;
        BoneMatrixOffset = 0x150; //基矩阵 0xcf0 SoftObjectPath MeshAssetPath;
        BoneOffset = 0x580; //骨骼 //584 StaticMesh* StaticMesh;
        isBotOffset = 0x684; //人机 bool bIsAI;
        PlayerNameOffset = 0x628;//人物名字 FString PlayerName;
        BoxAddressOffset = 0x5c4;//物品地址数组 PickUpItemData[] PickUpDataList;
        IDNumberOffset = BoxAddressOffset + 4;//盒子物品数量
        vehicleCommonComponentOffset = 0x6b4; //汽车配置指针 VehicleCommonComponent* VehicleCommon;
        vehicleHpOffset = 0x12c;//汽车血量 Class: VehicleCommonComponent.VehicleComponent.ActorComponent.Object float HP;
        vehicleFuelOffset = 0x144; //汽车油量3 float Fuel;
        gunOffset = 0xc00;//枪偏移 射击武器实体 ShootWeaponEntity* ShootWeaponEntityComp;
        bulletVelocityOffset = 0xb98;//枪子弹速度 float BulletFireSpeed;
        resistanceOffset = 0x140;//枪口上抬 float AccessoriesVRecoilFactor;
        fireButtonOffset = 0x2584; //开火键 bool bIsPressingFireBtn;
        adsOffset = 0xa68;//开镜 bool bIsGunADS;
        lastUpdateStatusKeyListOffset = 0x1c60;//Cur 0x1bf0 AnimStatusKeyList LastUpdateStatusKeyList;
        equipWeaponOffset = 0x1C; //STExtraWeapon* EquipWeapon; 装备武器
        shootModeOffset = 0xa80; //byte ShootMode; 模式
        weaponOffset = lastUpdateStatusKeyListOffset + equipWeaponOffset; //0x1BEC;//武器
        holdingStateOffset = weaponOffset - 0x10;// 0x1BDC;//手持武器状态
        //relativeScale3DOffset = 0x140;//class: SceneComponent Vector RelativeScale3D;
        //boneLocationOffset =relativeScale3DOffset + 0xC + 0x4 + 0x0;  //SoftObjectPath MeshAssetPath;
        perLODBiasTypeInfoOffset = 0x30; //+ 骨骼30
        playerCameraManagerOffset = 0x35c; //PlayerCameraManager* PlayerCameraManager; 相机
        weaponIDOffset = 0x684;//武器ID 68c
        weaponAmmoOffset = 0xa74;//剩余子弹 int CurBulletNumInClip;
        weaponClipOffset = weaponAmmoOffset + 0x4;//弹夹 int CurMaxBulletNumInOneClip;

        bIsSkipCheckMeshRotateOffset = 0x1784;//bool bIsSkipCheckMeshRotate;
        vectorOffset = bIsSkipCheckMeshRotateOffset + 4; //向量

        bDeadOffset = 0x8f0;//判断死亡 bool bDead;
        STExtraPlayerStateOffset = 0x19fc;//判断状态 STExtraPlayerState* STExtraPlayerState;
        CustomSpringArmComponentOffset = 0x111c;//头部 CustomSpringArmComponent* SpringArmComp;
        MaxDistFromMainCharOffset = 0x2664;//判断人 float MaxDistFromMainChar;
        LineOfSightToOffset = 0x3D4;
        AddYawInputOfffset = 0x448;
        AddPitchInputOffset = 0x444;

        componentVelocityOffset = 0x1b0;//向量 Vector ComponentVelocity;
        controlRotationOffset = 0x318;//Rotator ControlRotation;
        pawnOffset = 0x300;//class: Controller(can use Character)

    }

    void china64Offset() {
        BoxItemSize = 56;
        UObjectToClassPrivate = 0x10;
        UStructToSuperStruct = 0x30;

        //Class: ULevel
        ULevelToAActors = 0xA0;
        ULevelToAActorsCount = 0xA0 + 0x8;
        PointerSize = 0x8;
        FNameEntryToNameString = 0x10 - 0x2;
        UObjectToInternalIndex = 0xC + 0xC;


        dw_CameraCacheEntry = 0x460;//CameraCacheEntry CameraCache;
        dw_MinimalViewInfo = 0x10; //MinimalViewInfo POV;
        dw_Location = 0x0;//class MinimalViewInfo  Vector Location;
        dw_Rotation = 0x18; // class MinimalViewInfo Rotator Rotation;
        dw_FOV = 0x24; //class MinimalViewInfo float FOV;
        GNames = 0xA88C060;
        GWorld = 0xAA5D468;


        persistentLevelOffset = 0x30; //Level* PersistentLevel;
        netDriverOffset = 0x38; //NetDriver* NetDriver;
        serverConnectionOffset = 0x78; //NetConnection* ServerConnection;
        playerControllerOffset = 0x30; //PlayerController* PlayerController;
        uMyObjectOffset = 0x458; //Pawn* AcknowledgedPawn;//[Offset: 0x458, Size: 8]
        //0x3f0;//自身0x20b8 +0x28 0x458;  //自身偏移 STExtraPlayerState* STExtraPlayerState;
        pawnOffset = 0x3f0;//class: Controller(can use Character)//自身

        RotationAngleOffset = 0x118;//旋转角
        PlayerControllerOffset = 0x3560;//STExtraPlayerController* STPlayerController;
        CoordinatePointerOffset = 0x1d8;//对象坐标指针 SceneComponent* RootComponent;
        CoordinateOffset = 0x1C0;//0x18c; //坐标偏移 Vector RelativeLocation;//[Offset: 0x18c, Size: 12]
        StateOffset = 0xd80;//人物状态 PawnStateRepSyncData PawnStateRepSyncData; uint64 CurrentStates;
        TeamIDOffset = 0x8c0;// 队伍 Class: UAECharacter.Character.Pawn.Actor.Object 队伍 int TeamID;
        HealthOffset = 0xba8;//血量 float Health;
        maxHealthOffset = 0xbb0;//最大血量
        BoneAddrOffset = 0x470;//阵列  SkeletalMeshComponent* Mesh;
        BoneMatrixOffset = 0x1b0; //基矩阵 0xcf0 SoftObjectPath MeshAssetPath;
        BoneOffset = 0x6E8; //骨骼 //584 StaticMesh* StaticMesh;

        isBotOffset = 0x8dc; //人机 bool bIsAI;
        PlayerNameOffset = 0x850;//人物名字 FString PlayerName;
        BoxAddressOffset = 0x730;//物品地址数组 PickUpItemData[] PickUpDataList;
        IDNumberOffset = BoxAddressOffset + PointerSize;//盒子物品数量
        vehicleCommonComponentOffset = 0x8b0; //汽车配置指针 VehicleCommonComponent* VehicleCommon;
        vehicleHpOffset = 0x198;//汽车血量 Class: VehicleCommonComponent.VehicleComponent.ActorComponent.Object float HP;
        vehicleFuelOffset = 0x1B8; //汽车油量3 float Fuel;
        gunOffset = 0xec0;//枪偏移 射击武器实体 ShootWeaponEntity* ShootWeaponEntityComp;
        bulletVelocityOffset = 0xc4c;//枪子弹速度 float BulletFireSpeed;
        resistanceOffset = 0xfb8;//枪口上抬 float AccessoriesVRecoilFactor;
        fireButtonOffset = 0x3158; //开火键 bool bIsPressingFireBtn;
        adsOffset = 0xe18;//开镜 bool bIsGunADS;
        lastUpdateStatusKeyListOffset = 0x24d8;//Cur 0x1bf0 AnimStatusKeyList LastUpdateStatusKeyList;
        equipWeaponOffset = 0x20; //STExtraWeapon* EquipWeapon; 装备武器
        shootModeOffset = 0xd24; //byte ShootMode; 模式
        weaponOffset = lastUpdateStatusKeyListOffset + equipWeaponOffset; //0x1BEC;//武器
        holdingStateOffset = weaponOffset - 0x14;// 0x1BDC;//手持武器状态
        //relativeScale3DOffset = 0x140;//class: SceneComponent Vector RelativeScale3D;
        //boneLocationOffset =relativeScale3DOffset + 0xC + 0x4 + 0x0;  //SoftObjectPath MeshAssetPath;
        perLODBiasTypeInfoOffset = 0x30; //+ 骨骼30
        playerCameraManagerOffset = 0x478; //PlayerCameraManager* PlayerCameraManager; 相机
        weaponIDOffset = 0x7d0;//武器ID 7dc
        weaponAmmoOffset = 0xd18;//剩余子弹 int CurBulletNumInClip;
        weaponClipOffset = weaponAmmoOffset + 0x4;//弹夹 int CurMaxBulletNumInOneClip;

        bIsSkipCheckMeshRotateOffset = 0x1e30;//bool bIsSkipCheckMeshRotate;
        vectorOffset = bIsSkipCheckMeshRotateOffset + 0x18; //人物跑动向量

        // vectorOffset = 0x1D74;//向量
        bDeadOffset = 0xc10;//判断死亡 bool bDead;
        STExtraPlayerStateOffset = 0x2190;//判断状态 STExtraPlayerState* STExtraPlayerState;
        CustomSpringArmComponentOffset = 0x1748;//头部 CustomSpringArmComponent* SpringArmComp;
        MaxDistFromMainCharOffset = 0x30b4;//判断人 float MaxDistFromMainChar;
        componentVelocityOffset = 0x210;//向量 Vector ComponentVelocity;

        controlRotationOffset = 0x418;//Rotator ControlRotation;


        ClientSetLocationOffset = 0x760;

        SetControlRotationOffset = 0x758;

        LineOfSightToOffset = 0x7A8;

        AddYawInputOfffset = 0x890; //0x448;

        AddPitchInputOffset = 0x888;//0x444;

        PoseStateOffset = 0x1408;//[Offset: 0x1408, Size: 1]

        pitch_off = 0x2c7183c;
        yaw_off = 0x2c71850;
        line_off = 0x5eec690;
        GetGetActorBoundsOffset = 0x5C59598;
        GetBoneNameOffset = 0x5EBA718;
        GetBoneLocationOffset = 0x5EBC248;
    }

};

#endif //PUG_NEW_OFFSETS_H
