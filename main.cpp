//#define _CRT_SECURE_NO_WARNINGS

#include <dxgi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>      // für std::round
#include <dxgi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <regex>
//#pragma comment(lib, "dxgi.lib")

// Dein hartkodierter Config-Block mit Marker {POOLSIZE}
const std::string configTemplate = R"(
[Core.System]
Paths=../../../Engine/Content
Paths=%GAMEDIR%Content
Paths=../../../OblivionRemastered/Plugins/AltarBI/Content
Paths=../../../OblivionRemastered/Plugins/AltarTesting/Content
Paths=../../../OblivionRemastered/Plugins/AltarValidation/Content
Paths=../../../OblivionRemastered/Plugins/AltarWindows/Content
Paths=../../../OblivionRemastered/Plugins/AnimationNode/Content
Paths=../../../OblivionRemastered/Plugins/AssetCompletionCommands/Content
Paths=../../../OblivionRemastered/Plugins/AudioTool/Content
Paths=../../../OblivionRemastered/Plugins/ConstructionSet64/Content
Paths=../../../OblivionRemastered/Plugins/EmissiveLight/Content
Paths=../../../OblivionRemastered/Plugins/HoudiniNiagara/Content
Paths=../../../OblivionRemastered/Plugins/IGToolsTCP/Content
Paths=../../../OblivionRemastered/Plugins/LibXL/Content
Paths=../../../OblivionRemastered/Plugins/MapImporterCommands/Content
Paths=../../../OblivionRemastered/Plugins/OblivionThirdParty/Content
Paths=../../../OblivionRemastered/Plugins/OnlineSubsystemBethesdaNet/Content
Paths=../../../OblivionRemastered/Plugins/QMPUnrealIntegration/Content
Paths=../../../OblivionRemastered/Plugins/SideFX_Labs/Content
Paths=../../../OblivionRemastered/Plugins/SteeringBehaviors/Content
Paths=../../../OblivionRemastered/Plugins/TA_EditorBlueprintExtension/Content
Paths=../../../OblivionRemastered/Plugins/UE5AltarPairing/Content
Paths=../../../OblivionRemastered/Plugins/ue5-goliath-plugin-dev/Content
Paths=../../../OblivionRemastered/Plugins/VActorStateMachine/Content
Paths=../../../OblivionRemastered/Plugins/VCommonUIEnhancedInputNavigation/Content
Paths=../../../OblivionRemastered/Plugins/VWorldPartitionUtils/Content
Paths=../../../OblivionRemastered/Plugins/VirtuosCoreEditor/Content
Paths=../../../OblivionRemastered/Plugins/VirtuosEditorAutomationTesting/Content
Paths=../../../OblivionRemastered/Plugins/VirtuosProductionTools/Content
Paths=../../../OblivionRemastered/Plugins/WwiseNiagara/Content
Paths=../../../OblivionRemastered/Plugins/Wwise/Content
Paths=../../../Engine/Plugins/2D/Paper2D/Content
Paths=../../../Engine/Plugins/Animation/ACLPlugin/Content
Paths=../../../Engine/Plugins/Animation/AnimationWarping/Content
Paths=../../../Engine/Plugins/Animation/ControlRigSpline/Content
Paths=../../../Engine/Plugins/Animation/ControlRig/Content
Paths=../../../Engine/Plugins/Animation/DeformerGraph/Content
Paths=../../../Engine/Plugins/Animation/IKRig/Content
Paths=../../../Engine/Plugins/Animation/MotionWarping/Content
Paths=../../../Engine/Plugins/Animation/RigLogic/Content
Paths=../../../Engine/Plugins/Bridge/Content
Paths=../../../Engine/Plugins/Compositing/OpenColorIO/Content
Paths=../../../Engine/Plugins/Developer/AnimationSharing/Content
Paths=../../../Engine/Plugins/Developer/Concert/ConcertSync/ConcertSyncClient/Content
Paths=../../../Engine/Plugins/Editor/BlueprintHeaderView/Content
Paths=../../../Engine/Plugins/Editor/ConsoleVariablesEditor/Content
Paths=../../../Engine/Plugins/Editor/GeometryMode/Content
Paths=../../../Engine/Plugins/Editor/ModelingToolsEditorMode/Content
Paths=../../../Engine/Plugins/Editor/ObjectMixer/LightMixer/Content
Paths=../../../Engine/Plugins/Editor/ObjectMixer/ObjectMixer/Content
Paths=../../../Engine/Plugins/Editor/SpeedTreeImporter/Content
Paths=../../../Engine/Plugins/Editor/UVEditor/Content
Paths=../../../Engine/Plugins/Enterprise/DatasmithContent/Content
Paths=../../../Engine/Plugins/Enterprise/GLTFExporter/Content
Paths=../../../Engine/Plugins/Experimental/ChaosCaching/Content
Paths=../../../Engine/Plugins/Experimental/ChaosClothAssetEditor/Content
Paths=../../../Engine/Plugins/Experimental/ChaosClothAsset/Content
Paths=../../../Engine/Plugins/Experimental/ChaosClothEditor/Content
Paths=../../../Engine/Plugins/Experimental/ChaosNiagara/Content
Paths=../../../Engine/Plugins/Experimental/ChaosSolverPlugin/Content
Paths=../../../Engine/Plugins/Experimental/Dataflow/Content
Paths=../../../Engine/Plugins/Experimental/FullBodyIK/Content
Paths=../../../Engine/Plugins/Experimental/GeometryCollectionPlugin/Content
Paths=../../../Engine/Plugins/Experimental/GeometryScripting/Content
Paths=../../../Engine/Plugins/Experimental/ImpostorBaker/Content
Paths=../../../Engine/Plugins/Experimental/Landmass/Content
Paths=../../../Engine/Plugins/Experimental/PythonScriptPlugin/Content
Paths=../../../Engine/Plugins/Experimental/ToolPresets/Content
Paths=../../../Engine/Plugins/Experimental/VirtualHeightfieldMesh/Content
Paths=../../../Engine/Plugins/Experimental/Volumetrics/Content
Paths=../../../Engine/Plugins/Experimental/Water/Content
Paths=../../../Engine/Plugins/FX/NiagaraFluids/Content
Paths=../../../Engine/Plugins/FX/Niagara/Content
Paths=../../../Engine/Plugins/Fab/Content
Paths=../../../Engine/Plugins/Interchange/Runtime/Content
Paths=../../../Engine/Plugins/Marketplace/FSR3/FSR3/Content
Paths=../../../Engine/Plugins/Marketplace/XeSS/Content
Paths=../../../Engine/Plugins/Marketplace/nvidia/DLSS/DLSS/Content
Paths=../../../Engine/Plugins/Media/MediaCompositing/Content
Paths=../../../Engine/Plugins/Media/MediaPlate/Content
Paths=../../../Engine/Plugins/MovieScene/MovieRenderPipeline/Content
Paths=../../../Engine/Plugins/MovieScene/SequencerScripting/Content
Paths=../../../Engine/Plugins/Runtime/AudioSynesthesia/Content
Paths=../../../Engine/Plugins/Runtime/AudioWidgets/Content
Paths=../../../Engine/Plugins/Runtime/CommonUI/Content
Paths=../../../Engine/Plugins/Runtime/GeometryProcessing/Content
Paths=../../../Engine/Plugins/Runtime/HDRIBackdrop/Content
Paths=../../../Engine/Plugins/Runtime/HairStrands/Content
Paths=../../../Engine/Plugins/Runtime/HoudiniEngine/Content
Paths=../../../Engine/Plugins/Runtime/MeshModelingToolset/Content
Paths=../../../Engine/Plugins/Runtime/Metasound/Content
Paths=../../../Engine/Plugins/Runtime/ResonanceAudio/Content
Paths=../../../Engine/Plugins/Runtime/RigVM/Content
Paths=../../../Engine/Plugins/Runtime/Synthesis/Content
Paths=../../../Engine/Plugins/Runtime/WaveTable/Content
Paths=../../../Engine/Plugins/Runtime/WebBrowserWidget/Content
Paths=../../../Engine/Plugins/TraceUtilities/Content
Paths=../../../Engine/Plugins/VirtualProduction/Takes/Content

[/Script/Engine.Engine]
bAllowMultiThreadedShaderCompile=1


[/Script/D3D12RHI.D3D12Options]
D3D12.AFRUseFramePacing=1
D3D12.Aftermath=0
d3d12.AllowPoolAllocateIndirectArgBuffers=1
D3D12.AsyncDeferredDeletion=1
D3D12.ForceThirtyHz=0
D3D12.InsertOuterOcclusionQuery=1
D3D12.MaxCommandsPerCommandList=20000
D3D12.MaximumFrameLatency=3
D3D12.PSO.DiskCache=1
D3D12.PSO.DriverOptimizedDiskCache=1
D3D12.ResidencyManagement=1
D3D12.VRAMBufferPoolDefrag=1
d3d12.VRAMBufferPoolDefrag.MaxCopySizePerFrame=33554432
D3D12.VRAMTexturePoolDefrag=1
d3d12.VRAMTexturePoolDefrag.MaxCopySizePerFrame=33554432
D3D12.ZeroBufferSizeInMB=16
r.D3D.ForceDXC=1
r.D3D12.LightweightDRED=0
r.D3D12.RayTracing.AllowSpecializedStateObjects=0

[/Script/Engine.StreamingSettings]
r.Streaming.FullyLoadUsedTextures=0
r.Streaming.PredictiveBoost=1
r.Streaming.PredictiveBoostHintSize=512
r.Streaming.UseMaterialDataStreaming=1
s.AdaptiveAddToWorld.Enabled=1
s.AsyncLoadingThreadEnabled=1
s.AsyncLoadingThreadPriority=2
s.AsyncLoadingTimeLimit=3.0
s.IoDispatcherBufferMemoryMB=64
s.IoDispatcherCacheSizeMB=2048
s.IoDispatcherDecompressionWorkerCount=4
s.MaxIncomingRequestsToStall=1
s.MaxLevelRequestsAtOnceWhileInMatch=4
s.MaxPrecacheRequestsInFlight=8
s.MaxReadyRequestsToStallMB=0
s.MinBulkDataSizeForAsyncLoading=0
s.PriorityAsyncLoadingExtraTime=0.0
s.PriorityLevelStreamingActorsUpdateExtraTime=0.0
s.ProcessPrestreamingRequests=1

[/Script/Niagara.NiagaraSettings]
fx.MaxNiagaraGPUParticlesSpawnPerFrame=120000
fx.Niagara.GpuComputeDebug.DrawDebugEnabled=0
fx.Niagara.WarnComponentRenderCount=999999
fx.NiagaraAllowRuntimeScalabilityChanges=1
fx.NiagaraBatcher.FreeBufferEarly=0
fx.NiagaraDataBufferMinSize=2048
fx.NiagaraRuntimeCycleHistorySize=30

[/Script/Engine.GarbageCollectionSettings]
gc.AllowParallelGC=1
gc.CreateGCClusters=1
gc.MultithreadedDestructionEnabled=1

[ShaderCompiler]
bAllowAsynchronousShaderCompiling=1
bAllowCompilingThroughWorkerThreads=1
bAsyncShaderCompileWorkerThreads=1
bEnableOptimizedShaderCompilation=1
MaxShaderJobBatchSize=150
MaxShaderJobs=1000
NumUnusedShaderCompilingThreads=2
r.GPUDefrag.AllowOverlappedMoves=1
r.GPUDefrag.EnableTimeLimits=1


[/Script/Engine.RendererSettings]
r.FilmGrain=0                                
r.Tonemapper.GrainQuantization=0               
r.SceneColorFringe.Max=0                      
r.SceneColorFringeQuality=0        
r.MaxAnisotropy=16
r.Tonemapper.Sharpen=0.2
r.DefaultFeature.AntiAliasing=2
r.PostProcessAAQuality=6
r.Reflections.Denoiser.TemporalAccumulation=1
r.Reflections.Denoiser=2
r.TemporalAA.Algorithm=1
r.TemporalAA.Upsampling=1
r.TemporalAACurrentFrameWeight=0.2
r.TemporalAAFilterSize=1.0
r.TemporalAASamples=4
r.TemporalAASharpness=0
r.BasePassForceOutputsVelocity=1
r.FidelityFX.FSR.RCAS=0
r.FidelityFX.FSR2.AutoExposure=1
r.FidelityFX.FSR2.Sharpness=0.0
r.FidelityFX.FSR3.AutoExposure=1
r.FidelityFX.FSR3.Sharpness=0.0
r.FidelityFX.FSR4.AutoExposure=1
r.FidelityFX.FSR4.Sharpness=0.0
r.NGX.DLSS.AutoExposure=1
r.NGX.DLSS.EnableAutoExposure=1
r.NGX.DLSS.PreferNISSharpen=0
r.NGX.DLSS.Sharpness=0
r.NIS.Enable=0
r.XeSS.AutoExposure=1
r.AOAsyncBuildQueue=1
r.AsyncCompute.AdaptiveBuffer=1
r.AsyncCompute.ParallelDispatch=1
r.AsyncCompute=1
r.AsyncCreateLightPrimitiveInteractions=1
r.AsyncPipelineCompile=1
r.CompileShadersForDevelopment=0
r.CookOutUnusedDetailModeComponents=1
r.D3D.RemoveUnusedInterpolators=1
r.EnableAsyncComputeTranslucencyLightingVolumeClear=1
r.EnableAsyncComputeVolumetricFog=1
r.UseAsyncShaderPrecompilation=1
r.D3D12.GPUCrashDebuggingMode=0
r.D3D12.GPUTimeout=0
r.gpucrash.collectionenable=0
r.gpucrash.datadepth=0
r.GPUCrashDebugging.Aftermath.Callstack=0
r.GPUCrashDebugging.Aftermath.Markers=0
r.GPUCrashDebugging=0
r.GPUCrashDump=0
r.GPUDefrag.MaxRelocations=10
r.GraphicsThread.EnableBackgroundThreads=1
r.GraphicsThread.UseThreadedDestruction=1
r.GTSyncType=2
r.RHICmdUseParallelAlgorithms=1
r.RHICmdUseThread=1
r.RHIThread.Priority=2
r.RHIThread=1
RHI.ResourceTableCaching=1
RHI.SyncAllowEarlyKick=1
RHI.SyncThreshold=999
r.RHI.UseParallelDispatch=1
r.RHICmdBuffer.EnableThreadedCompletion=1
r.RHICmdBypass=0
r.RDG.AsyncClothTick=1
r.RDG.AsyncPipelineCompile=1
r.RDG.ParallelExecute=1
r.RDG.ParallelUpdateRenderGraph=1
r.IO.UseDirectStorage=1
r.IO.VirtualTextures=1
r.Lumen.DiffuseIndirect.Allow=1
r.Lumen.DiffuseIndirect.AsyncCompute=1
r.Lumen.Reflections.RadianceCache=1
r.Lumen.Reflections.Temporal=1
r.Lumen.ScreenProbeGather.AsyncCompute=1
r.Lumen.ScreenProbeGather.TemporalFilterProbes=1
r.LumenScene.DirectLighting.OffscreenShadowing.TraceMeshSDFs=0
r.LumenScene.GPUDrivenUpdate=1
r.LumenScene.Lighting.AsyncCompute=1
r.LumenScene.MeshCardsPerTask=448
r.LumenScene.ParallelUpdate=1
r.LumenScene.PrimitivesPerTask=448
r.LumenScene.PropagateGlobalLightingChange=0
r.LumenScene.SurfaceCache.AtlasSize=2048
r.LumenScene.SurfaceCache.CardCapturesPerFrame=150
r.LumenScene.SurfaceCache.Feedback.MinPageHits=32.0
r.LumenScene.SurfaceCache.Feedback.UniqueElements=2048
r.LumenScene.SurfaceCache.NumFramesToKeepUnusedPages=128
r.LumenScene.SurfaceCache.RemovesPerFrame=128
r.Nanite.LargePageRectThreshold=256
r.Nanite.MaterialVisibility.Async=1
r.Nanite.MaterialVisibility=1
r.Nanite.Streaming.MaxPageInstallsPerFrame=32
r.Nanite.Streaming.StreamingPoolSize=768
r.Nanite.VSMMeshShaderRasterization=1
r.ParallelAnimationCacheConversion=1
r.ParallelAnimationCacheConversionAsync=1
r.ParallelAnimationCacheStreaming=1
r.ParallelAnimationCompression=1
r.ParallelAnimationCompressionAsync=1
r.ParallelAnimationEvaluation=1
r.ParallelAnimationRetargeting=1
r.ParallelAnimationRetargetingAsync=1
r.ParallelAnimationStreaming=1
r.ParallelAnimationStreamingAsync=1
r.ParallelAnimationUpdate=1
r.ParallelAsyncComputeSkinCache=1
r.ParallelAsyncComputeTranslucency=1
r.ParallelBasePass=1
r.ParallelBatchDispatch=1
r.ParallelCulling=1
r.ParallelDestruction=1
r.ParallelDistanceField=1
r.ParallelDistributedScene=1
r.ParallelGraphics=1
r.ParallelInitViews=1
r.ParallelLandscapeLayerUpdate=1
r.ParallelLandscapeSplatAtlas=1
r.ParallelLandscapeSplineSegmentCalc=1
r.ParallelLandscapeSplineUpdate=1
r.ParallelLightingBuild=1
r.ParallelLightingComposition=1
r.ParallelLightingInject=1
r.ParallelLightingPropagation=1
r.ParallelLightingSetup=1
r.ParallelMeshBuildUseJobCulling=1
r.ParallelMeshBuildUseJobMerging=1
r.ParallelMeshDrawCommands=1
r.ParallelMeshMerge=1
r.ParallelMeshProcessing=1
r.ParallelNavBoundsCalc=1
r.ParallelNavBoundsInit=1
r.ParallelNavBoundsUpdate=1
r.ParallelNavOctreeUpdate=1
r.ParallelParticleUpdate=1
r.ParallelPhysicsScene=1
r.ParallelPhysicsStepAsync=1
r.ParallelPostProcessing=1
r.ParallelPrePass=1
r.ParallelReflectionCaptures=1
r.ParallelReflectionEnvironment=1
r.ParallelRendering=1
r.ParallelRenderUploads=1
r.ParallelSceneCapture=1
r.ParallelSceneColorGather=1
r.ParallelShaderCompile=1
r.ParallelSkeletalClothBoundsCalc=1
r.ParallelSkeletalClothGather=1
r.ParallelSkeletalClothPrepareSim=1
r.ParallelSkeletalClothSimulate=1
r.ParallelSkeletalClothSkinning=1
r.ParallelSkeletalClothUpdate=1
r.ParallelSkeletalClothUpdateBounds=1
r.ParallelSkeletalClothUpdateVerts=1
r.ParallelTaskShaderCompilation=1
r.ParallelTonemapping=1
r.ParallelTranslucency=1
r.ParallelVelocity=1
r.ParallelZPrepass=1
r.PipelineStateCache.AsyncCompileAfterTypes=1
r.PreTileTextures=1
r.RDG.AsyncCompute=1
r.Renderer.UseGPUInstancing=1
r.RenderThread.EnableTaskGraphThread=1
r.RenderThread.Priority=2
r.ShaderLibrary.PrintExtendedStats=0
r.Shadow.CacheWPOPrimitives=1
r.Shadow.CSMCaching=1
r.Shadow.DetectVertexShaderLayerAtRuntime=1
r.Shadow.FadeExponent=0.75
r.Shadow.UnbuiltPreviewInGame=0
r.Shadow.Virtual.Cache.AllocateViaLRU=1
r.Shadow.Virtual.Cache.InvalidateUseHZB=0
r.Shadow.Virtual.NonNanite.IncludeInCoarsePages=0
r.Shadow.Virtual.UseHZB=1
r.SkyAtmosphere.AerialPerspectiveLUT.FastApplyOnOpaque=1
r.SSS.Burley.EnableProfileIdCache=1
r.VRS.ContrastAdaptiveShading=0
r.VRS.Enable=1
r.VRS.EnableImage=1
r.VRS.Tier=2
r.VT.ParallelFeedbackTasks=1
r.TextureStreaming.DiscardUnusedMips=1
r.Streaming.PoolSize={POOLSIZE}
r.TextureStreaming.UseBackgroundThreadPool=1
r.TextureStreaming.UseDeferredLock=1
r.TextureStreaming=1
r.ThreadPool.BackgroundThreadPriority=0
r.ThreadPool.EnableBackgroundThreads=1
r.ThreadPool.EnableHighPriorityThreads=1
r.Streaming.AmortizeCPUToGPUCopy=1
r.Streaming.DefragDynamicBounds=1
r.Streaming.FramesForFullUpdate=2
r.Streaming.LimitPoolSizeToVRAM=1
r.Streaming.MaxMipLevelReduction=0
r.Streaming.MinMipForSplitRequest=0
r.Streaming.ParallelRenderAssetsNumWorkgroups=4
r.Streaming.StressTest.ExtraAsyncLatency=0
r.Streaming.UseAsyncRequestsForDDC=1
r.Streaming.UseBackgroundThreadPool=1
r.Streaming.UseNewMetrics=1
r.Streaming.UsePerTextureBias=1
r.AllowMultiThreadedShaderCreation=1
r.DFShadowAsyncCompute=1
r.DontLimitOnBattery=1
r.DumpGPU=0
r.Emitter.FastPoolEnable=1
r.EnableDebugSpam_GetObjectPositionAndScale=0
r.EnableMultiThreadedRendering=1
r.FinishCurrentFrame=0
r.ForceAllCoresForShaderCompiling=1
r.ForceDebugViewModes=2
r.ForceOcclusionQueryBatching=1
r.GeometryCollection.Nanite.AsyncCompute=1
r.GeometryCollection.Nanite=1
r.InstanceCulling.OcclusionCull=1
r.InstancedStaticMeshes.ForceRemoveAtSwap=1
r.LODFadeTime=0.75
r.NGX.LogLevel=0
r.NumBufferedOcclusionQueries=2
r.Occlusion.SingleRHIThreadStall=1
r.OneFrameThreadLag=1
r.pso.evictiontime=30
r.ShaderDrawDebug=0
r.Shaders.Optimize=1
r.SupportAllShaderPermutations=0
r.ThreadedShaderCompilation=1
r.UniformBufferPooling=1
r.Visibility.FrustumCull.UseSphereTestFirst=1
r.Visibility.TaskSchedule=0
r.VT.CsvStats=0

)";
const std::string configTemplate2 = R"(


[DevOptions.Shaders]
bAllowShaderCompilingWorker=1
bOptimizeForLocalShaderBuilds=1
bUseBackgroundCompiling=1
WorkerThreadPriority=0

[Engine.InputSettings]
bDisableMouseAcceleration=1
bEnableMouseSmoothing=0
bViewAccelerationEnabled=0
RawMouseInputEnabled=1

[RenderingThread]
bAllowAsyncRenderThreadUpdates=1
bAllowThreadedRendering=1


[ShaderPipelineCache]
r.ShaderPipelineCache.AsyncCompileRate=32
r.ShaderPipelineCache.BackgroundBatchSize=3
r.ShaderPipelineCache.BatchTime=4.0
r.ShaderPipelineCache.Enabled=1
r.ShaderPipelineCache.PrecompileBatchTime=5
r.ShaderPipelineCache.PrecompileFrameTime=20
r.ShaderPipelineCache.StartupCache=1


[Physics]
p.AsyncSceneEnabled=1
p.Chaos.PerParticleCollision.ISPC=1
p.Chaos.Spherical.ISPC=1
p.Chaos.Spring.ISPC=1
p.Chaos.TriangleMesh.ISPC=1
p.Chaos.VelocityField.ISPC=1
p.Chaos.VisualDebuggerEnable=0

[/Script/Engine.AudioSettings]
PlatformFormat=OGG
PlatformStreamingFormat=OGG

[/Script/AKAudio.AkSettings]
bEnableMultiCoreRendering=1

[CrashReportClient]
bAgreeToCrashUpload=0
bImplicitSend=0

[Core.Log]
Global=none
LogAI=none
LogAnalytics=none
LogAnimation=none
LogBlueprint=none
LogConfig=none
LogCore=none
LogInput=none
LogInteractiveProcess=none
LogLinker=none
LogMemory=none
LogNetwork=none
LogOnline=none
LogOnlineEntitlement=none
LogOnlineEvents=none
LogOnlineFriend=none
LogOnlineGame=none
LogOnlineIdentity=none
LogOnlinePresence=none
LogOnlineSession=none
LogOnlineTitleFile=none
LogOnlineUser=none
LogPakFile=none
LogPhysics=none
LogPluginManager=none
LogRenderer=none
LogRenderTargetPool=none
LogShaderCompiler=none
LogStreaming=none
LogTelemetry=none
LogTemp=none
LogUObject=none

[Engine.ErrorHandling]
bPromptForRemoteDebugging=0
bPromptForRemoteDebugOnEnsure=0

[/Script/WInstrumentedProfilersSettings.WTelemetrySettings]
bEnableTelemetry=0

[FATHydraCrashHandler]
LogCrashReportHydra=0
LogCrashUploader=0


[/Script/Engine.NetworkSettings]
net.AllowAsyncLoading=1

[ConsoleVariables]
r.FilmGrain=0                                
r.Tonemapper.GrainQuantization=0              
r.NT.Lens.ChromaticAberration.Intensity=0    
r.SceneColorFringe.Max=0                      
r.SceneColorFringeQuality=0                  
a.ForceParallelAnimUpdate=1
ai.DestroyNavDataInCleanUpAndMarkPendingKill=0
AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=0
Async.ParallelFor.YieldingTimeout=99
AttemptStuckThreadResuscitation=1
au.BakedAnalysisEnabled=0
au.DisableParallelSourceProcessing=0
au.voip.AlwaysPlayVoiceComponent=0
AudioThread.BatchAsyncBatchSize=256
AudioThread.EnableBatchProcessing=1
bAgreeToCrashUpload=0
bAllowAsynchronousShaderCompiling=1
bAllowAsyncRenderThreadUpdates=1
bAllowCompilingThroughWorkerThreads=1
bAllowMultiThreadedAnimationUpdate=1
bAllowMultiThreadedShaderCompile=1
bAllowShaderCompilingWorker=1
bAllowThreadedRendering=1
bAsyncShaderCompileWorkerThreads=1
bCanBlueprintsTickByDefault=0
bDisableMouseAcceleration=1
bEnableMouseSmoothing=0
bEnableMultiCoreRendering=1
bEnableOptimizedShaderCompilation=1
bEnableTelemetry=0
bImplicitSend=0
bOptimizeAnimBlueprintMemberVariableAccess=1
bOptimizeForLocalShaderBuilds=1
bPromptForRemoteDebugging=0
bPromptForRemoteDebugOnEnsure=0
bSmoothFrameRate=0
bSupportsGPUScene=1
bSupportsWaveOperations=1
bUseAsyncComputeContext=1
bUseBackgroundCompiling=1
bViewAccelerationEnabled=0
csv.trackWaitsGT=0
csv.trackWaitsRT=0
D3D12.AFRUseFramePacing=1
D3D12.Aftermath=0
d3d12.AllowPoolAllocateIndirectArgBuffers=1
D3D12.AsyncDeferredDeletion=1
D3D12.ForceThirtyHz=0
D3D12.InsertOuterOcclusionQuery=1
D3D12.MaxCommandsPerCommandList=20000
D3D12.MaximumFrameLatency=3
D3D12.PSO.DriverOptimizedDiskCache=1
D3D12.ResidencyManagement=1
D3D12.VRAMBufferPoolDefrag=1
d3d12.VRAMBufferPoolDefrag.MaxCopySizePerFrame=33554432
D3D12.VRAMTexturePoolDefrag=1
d3d12.VRAMTexturePoolDefrag.MaxCopySizePerFrame=33554432
D3D12.ZeroBufferSizeInMB=16
EnableMathOptimisations=1
foliage.MaxOcclusionQueriesPerComponent=8
foliage.MinOcclusionQueriesPerComponent=3
FX.AllowAsyncTick=1
fx.AllowFastPathFunctionLibrary=1
FX.BatchAsync=1
FX.BatchAsyncBatchSize=8
fx.DeferrPSCDeactivation=1
FX.EarlyScheduleAsync=1
fx.EnableCircularAnimTrailDump=0
fx.GPUSimulationTextureSizeX=512
fx.GPUSimulationTextureSizeY=512
FX.GPUSpawnWarningThreshold=99999999
FX.MaxCPUParticlesPerEmitter=750
FX.MaxGPUParticlesSpawnedPerFrame=60000
fx.MaxNiagaraGPUParticlesSpawnPerFrame=120000
fx.Niagara.AsyncCompute=1
fx.Niagara.DebugDraw.Enabled=0
fx.Niagara.GpuComputeDebug.DrawDebugEnabled=0
fx.Niagara.WarnComponentRenderCount=999999
fx.NiagaraAllowRuntimeScalabilityChanges=1
fx.NiagaraBatcher.FreeBufferEarly=0
fx.NiagaraDataBufferMinSize=2048
fx.NiagaraRuntimeCycleHistorySize=30
fx.ParticlePerfStats.Enabled=0
fx.ScalabilityManParallelThreshold=25
fx.ScalabilityMaxUpdatesPerFrame=25
fx.SkipVectorVMBackendOptimizations=0
gc.AllowParallelGC=1
gc.CreateGCClusters=1
gc.MultithreadedDestructionEnabled=1
GeometryCache.OffloadUpdate=1
grass.MaxAsyncTasks=8
grass.MaxCreatePerFrame=6
grass.MaxInstancesPerComponent=49152
grass.MinFramesToKeepGrass=60
grass.UseHaltonDistribution=1
landscape.RenderNanite=1
LevelSequence.InvalidBindingTagWarnings=0
LogAI=none
LogAnalytics=none
LogAnimation=none
LogBlueprint=none
LogConfig=none
LogCore=none
LogCrashReportHydra=0
LogCrashUploader=0
LogInput=none
LogInteractiveProcess=none
LogLinker=none
LogMemory=none
LogNetwork=none
LogOnline=none
LogOnlineEntitlement=none
LogOnlineEvents=none
LogOnlineFriend=none
LogOnlineGame=none
LogOnlineIdentity=none
LogOnlinePresence=none
LogOnlineSession=none
LogOnlineTitleFile=none
LogOnlineUser=none
LogPakFile=none
LogPhysics=none
LogPluginManager=none
LogRenderer=none
LogRenderTargetPool=none
LogShaderCompiler=none
LogStreaming=none
LogTelemetry=none
LogTemp=none
LogUObject=none
MaxShaderJobBatchSize=150
MaxShaderJobs=1000
memory.logGenericPlatformMemoryStats=0
net.AllowAsyncLoading=1
niagara.CreateShadersOnLoad=1
NumUnusedShaderCompilingThreads=2
p.AsyncSceneEnabled=1
p.Chaos.PerParticleCollision.ISPC=1
p.Chaos.Spherical.ISPC=1
p.Chaos.Spring.ISPC=1
p.Chaos.TriangleMesh.ISPC=1
p.Chaos.VelocityField.ISPC=1
p.Chaos.VisualDebuggerEnable=0
p.RemoveFarBodiesFromBVH=1
pakcache.CachePerPakFile=1
pakcache.MaxBlockMemory=384
pakcache.MaxRequestSizeToLowerLevellKB=3072
pakcache.MaxRequestsToLowerLevel=3
pakcache.NumUnreferencedBlocksToCache=20
pakcache.UseNewTrim=1
PlatformFormat=OGG
PlatformStreamingFormat=OGG
PoolSizeVRAMPercentage=70
r.AllowAsyncRenderThreadUpdatesDuringGamethreadUpdates=0
r.AllowMultiThreadedShaderCreation=1
r.AOAsyncBuildQueue=1
r.AsyncCompute.AdaptiveBuffer=1
r.AsyncCompute.ParallelDispatch=1
r.AsyncCompute=1
r.AsyncCreateLightPrimitiveInteractions=1
r.AsyncPipelineCompile=1
r.BasePassForceOutputsVelocity=1
r.CompileShadersForDevelopment=0
r.CookOutUnusedDetailModeComponents=1
r.CustomDepth=3
r.D3D.ForceDXC=1
r.D3D.RemoveUnusedInterpolators=1
r.D3D12.GPUCrashDebuggingMode=0
r.D3D12.GPUTimeout=0
r.D3D12.LightweightDRED=0
r.D3D12.RayTracing.AllowSpecializedStateObjects=0
r.DefaultFeature.AntiAliasing=2
r.DFShadowAsyncCompute=1
r.DistanceFields.ParallelUpdate=1
r.DontLimitOnBattery=1
r.DumpGPU=0
r.Emitter.FastPoolEnable=1
r.EnableAsyncComputeTranslucencyLightingVolumeClear=1
r.EnableAsyncComputeVolumetricFog=1
r.EnableDebugSpam_GetObjectPositionAndScale=0
r.EnableMultiThreadedRendering=1
r.FidelityFX.FSR.RCAS=0
r.FidelityFX.FSR2.AutoExposure=1
r.FidelityFX.FSR2.Sharpness=0.0
r.FidelityFX.FSR3.AutoExposure=1
r.FidelityFX.FSR3.Sharpness=0.0
r.FidelityFX.FSR4.AutoExposure=1
r.FidelityFX.FSR4.Sharpness=0.0
r.FinishCurrentFrame=0
r.ForceAllCoresForShaderCompiling=1
r.ForceDebugViewModes=2
r.ForceOcclusionQueryBatching=1
r.ForceParallelAnimUpdate=1
r.GeometryCollection.Nanite.AsyncCompute=1
r.GeometryCollection.Nanite=1
r.GPUDefrag.AllowOverlappedMoves=1
r.GPUDefrag.EnableTimeLimits=1
r.GPUDefrag.MaxRelocations=10
r.gpucrash.collectionenable=0
r.gpucrash.datadepth=0
r.GPUCrashDebugging.Aftermath.Callstack=0
r.GPUCrashDebugging.Aftermath.Markers=0
r.GPUCrashDebugging=0
r.GPUCrashDump=0
r.GraphicsThread.EnableBackgroundThreads=1
r.GraphicsThread.UseThreadedDestruction=1
r.GTSyncType=2
r.InstanceCulling.OcclusionCull=1
r.InstancedStaticMeshes.ForceRemoveAtSwap=1
r.IO.UseDirectStorage=1
r.IO.VirtualTextures=1
r.LODFadeTime=0.75
r.Lumen.DiffuseIndirect.Allow=1
r.Lumen.DiffuseIndirect.AsyncCompute=1
r.Lumen.Reflections.RadianceCache=1
r.Lumen.Reflections.Temporal=1
r.Lumen.ScreenProbeGather.AsyncCompute=1
r.Lumen.ScreenProbeGather.TemporalFilterProbes=1
r.LumenScene.DirectLighting.OffscreenShadowing.TraceMeshSDFs=0
r.LumenScene.GPUDrivenUpdate=1
r.LumenScene.Lighting.AsyncCompute=1
r.LumenScene.MeshCardsPerTask=448
r.LumenScene.ParallelUpdate=1
r.LumenScene.PrimitivesPerTask=448
r.LumenScene.PropagateGlobalLightingChange=0
r.LumenScene.SurfaceCache.AtlasSize=2048
r.LumenScene.SurfaceCache.CardCapturesPerFrame=150
r.LumenScene.SurfaceCache.Feedback.MinPageHits=32.0
r.LumenScene.SurfaceCache.Feedback.UniqueElements=2048
r.LumenScene.SurfaceCache.NumFramesToKeepUnusedPages=128
r.LumenScene.SurfaceCache.RemovesPerFrame=128
r.MaxAnisotropy=16
r.Nanite.LargePageRectThreshold=256
r.Nanite.MaterialVisibility.Async=1
r.Nanite.MaterialVisibility=1
r.Nanite.Streaming.MaxPageInstallsPerFrame=32
r.Nanite.Streaming.StreamingPoolSize=768
r.Nanite.VSMMeshShaderRasterization=1
r.NGX.DLSS.AutoExposure=1
r.NGX.DLSS.EnableAutoExposure=1
r.NGX.DLSS.PreferNISSharpen=0
r.NGX.DLSS.Sharpness=0
r.NGX.LogLevel=0
r.NIS.Enable=0
r.NumBufferedOcclusionQueries=2
r.Occlusion.SingleRHIThreadStall=1
r.OneFrameThreadLag=1
r.ParallelAnimationCacheConversion=1
r.ParallelAnimationCacheConversionAsync=1
r.ParallelAnimationCacheStreaming=1
r.ParallelAnimationCompression=1
r.ParallelAnimationCompressionAsync=1
r.ParallelAnimationEvaluation=1
r.ParallelAnimationRetargeting=1
r.ParallelAnimationRetargetingAsync=1
r.ParallelAnimationStreaming=1
r.ParallelAnimationStreamingAsync=1
r.ParallelAnimationUpdate=1
r.ParallelAsyncComputeSkinCache=1
r.ParallelAsyncComputeTranslucency=1
r.ParallelBasePass=1
r.ParallelBatchDispatch=1
r.ParallelCulling=1
r.ParallelDestruction=1
r.ParallelDistanceField=1
r.ParallelDistributedScene=1
r.ParallelGraphics=1
r.ParallelInitViews=1
r.ParallelLandscapeLayerUpdate=1
r.ParallelLandscapeSplatAtlas=1
r.ParallelLandscapeSplineSegmentCalc=1
r.ParallelLandscapeSplineUpdate=1
r.ParallelLightingBuild=1
r.ParallelLightingComposition=1
r.ParallelLightingInject=1
r.ParallelLightingPropagation=1
r.ParallelLightingSetup=1
r.ParallelMeshBuildUseJobCulling=1
r.ParallelMeshBuildUseJobMerging=1
r.ParallelMeshDrawCommands=1
r.ParallelMeshMerge=1
r.ParallelMeshProcessing=1
r.ParallelNavBoundsCalc=1
r.ParallelNavBoundsInit=1
r.ParallelNavBoundsUpdate=1
r.ParallelNavOctreeUpdate=1
r.ParallelParticleUpdate=1
r.ParallelPhysicsScene=1
r.ParallelPhysicsStepAsync=1
r.ParallelPostProcessing=1
r.ParallelPrePass=1
r.ParallelReflectionCaptures=1
r.ParallelReflectionEnvironment=1
r.ParallelRendering=1
r.ParallelRenderUploads=1
r.ParallelSceneCapture=1
r.ParallelSceneColorGather=1
r.ParallelShaderCompile=1
r.ParallelSkeletalClothBoundsCalc=1
r.ParallelSkeletalClothGather=1
r.ParallelSkeletalClothPrepareSim=1
r.ParallelSkeletalClothSimulate=1
r.ParallelSkeletalClothSkinning=1
r.ParallelSkeletalClothUpdate=1
r.ParallelSkeletalClothUpdateBounds=1
r.ParallelSkeletalClothUpdateVerts=1
r.ParallelTaskShaderCompilation=1
r.ParallelTonemapping=1
r.ParallelTranslucency=1
r.ParallelVelocity=1
r.ParallelZPrepass=1
r.PipelineStateCache.AsyncCompileAfterTypes=1
r.PostProcessAAQuality=6
r.PreTileTextures=1
r.pso.evictiontime=30
r.RDG.AsyncClothTick=1
r.RDG.AsyncCompute=1
r.RDG.AsyncPipelineCompile=1
r.RDG.ParallelExecute=1
r.RDG.ParallelUpdateRenderGraph=1
r.Reflections.Denoiser.TemporalAccumulation=1
r.Reflections.Denoiser=2
r.Renderer.UseGPUInstancing=1
r.RenderThread.EnableTaskGraphThread=1
r.RenderThread.Priority=2
r.RHI.UseParallelDispatch=1
r.RHICmdBuffer.EnableThreadedCompletion=1
r.RHICmdBypass=0
r.RHICmdUseParallelAlgorithms=1
r.RHICmdUseThread=1
r.RHIThread.Priority=2
r.RHIThread=1
r.ShaderCompiler.AllowDistributedCompilation=0
r.ShaderDrawDebug=0
r.ShaderLibrary.PrintExtendedStats=0
r.ShaderPipelineCache.AsyncCompileRate=32
r.ShaderPipelineCache.BackgroundBatchSize=3
r.ShaderPipelineCache.BatchSize=30
r.ShaderPipelineCache.BatchTime=4.0
r.ShaderPipelineCache.Enabled=1
r.ShaderPipelineCache.PrecompileBatchSize=30
r.ShaderPipelineCache.PrecompileBatchTime=5
r.ShaderPipelineCache.PrecompileFrameTime=20
r.ShaderPipelineCache.PreOptimizeEnabled=1
r.ShaderPipelineCache.ReportPSO=0
r.ShaderPipelineCache.StartupCache=1
r.Shaders.Optimize=1
r.Shaders.OptimizePreload=1
r.Shaders.Precompile=1
r.Shadow.CacheWPOPrimitives=1
r.Shadow.CSMCaching=1
r.Shadow.DetectVertexShaderLayerAtRuntime=1
r.Shadow.FadeExponent=0.75
r.Shadow.UnbuiltPreviewInGame=0
r.Shadow.Virtual.Cache.AllocateViaLRU=1
r.Shadow.Virtual.Cache.InvalidateUseHZB=0
r.Shadow.Virtual.Enable=1
r.Shadow.Virtual.NonNanite.IncludeInCoarsePages=0
r.Shadow.Virtual.UseHZB=1
r.SkyAtmosphere.AerialPerspectiveLUT.FastApplyOnOpaque=1
r.SSS.Burley.EnableProfileIdCache=1
r.Streaming.AmortizeCPUToGPUCopy=1
r.Streaming.DefragDynamicBounds=1
r.Streaming.FramesForFullUpdate=2
r.Streaming.FullyLoadUsedTextures=0
r.Streaming.LimitPoolSizeToVRAM=1
r.Streaming.MaxMipLevelReduction=0
r.Streaming.MinMipForSplitRequest=0
r.Streaming.ParallelRenderAssetsNumWorkgroups=4
r.Streaming.PredictiveBoost=1
r.Streaming.PredictiveBoostHintSize=512
r.Streaming.StressTest.ExtraAsyncLatency=0
r.Streaming.UseAsyncRequestsForDDC=1
r.Streaming.UseBackgroundThreadPool=1
r.Streaming.UseMaterialDataStreaming=1
r.Streaming.UseNewMetrics=1
r.Streaming.UsePerTextureBias=1
r.Streamline.TagUIColorAlpha=0
r.SupportAllShaderPermutations=0
r.TemporalAA.Algorithm=1
r.TemporalAA.Upsampling=1
r.TemporalAACurrentFrameWeight=0.2
r.TemporalAAFilterSize=1.0
r.TemporalAASamples=4
r.TemporalAASharpness=0
r.TextureStreaming.DiscardUnusedMips=1
r.TextureStreaming.UseBackgroundThreadPool=1
r.TextureStreaming.UseDeferredLock=1
r.TextureStreaming=1
r.ThreadedShaderCompilation=1
r.ThreadPool.BackgroundThreadPriority=0
r.ThreadPool.EnableBackgroundThreads=1
r.ThreadPool.EnableHighPriorityThreads=1
r.Tonemapper.Sharpen=0.2
r.UniformBufferPooling=1
r.UseAsyncShaderPrecompilation=1
r.Visibility.FrustumCull.UseSphereTestFirst=1
r.Visibility.TaskSchedule=0
r.VRS.ContrastAdaptiveShading=0
r.VRS.Enable=1
r.VRS.EnableImage=1
r.VRS.Tier=2
r.VT.CsvStats=0
r.VT.ParallelFeedbackTasks=1
r.XeSS.AutoExposure=1
RawMouseInputEnabled=1
RHI.ResourceTableCaching=1
RHI.SyncAllowEarlyKick=1
RHI.SyncThreshold=999
s.AdaptiveAddToWorld.Enabled=1
s.AsyncLoadingThreadEnabled=1
s.AsyncLoadingThreadPriority=2
s.AsyncLoadingTimeLimit=3.0
s.IoDispatcherBufferMemoryMB=64
s.IoDispatcherCacheSizeMB=2048
s.IoDispatcherDecompressionWorkerCount=4
s.MaxIncomingRequestsToStall=1
s.MaxLevelRequestsAtOnceWhileInMatch=4
s.MaxPrecacheRequestsInFlight=8
s.MaxReadyRequestsToStallMB=0
s.MinBulkDataSizeForAsyncLoading=0
s.PriorityAsyncLoadingExtraTime=0.0
s.PriorityLevelStreamingActorsUpdateExtraTime=0.0
s.ProcessPrestreamingRequests=1
Slate.bAllowThrottling=0
t.MaxFPS=0
TaskGraph.Enable=1
TaskGraph.ForkedProcessMaxWorkerThreads=4
TaskGraph.NumForegroundWorkers=4
TaskGraph.PrintBroadcastWarnings=0
TimerManager.DumpTimerLogResolveVirtualFunctions=0
TimerManager.DumpTimerLogSymbolNames=0
UseAllCores=1
WorkerThreadPriority=0
wp.Runtime.HLOD.WarmupNanite=0
wp.Runtime.HLOD.WarmupVTScaleFactor=1.0
wp.Runtime.LevelStreamingContinuouslyIncrementalGCWhileLevelsPendingPurgeForWP=128
wp.Runtime.MaxLoadingStreamingCells=16
wp.Runtime.UpdateStreamingStateTimeLimit=0.1

[WindowsApplication.Accessibility]
StickyKeysHotkey=True
ToggleKeysHotkey=True
FilterKeysHotkey=True
StickyKeysConfirmation=True
ToggleKeysConfirmation=True
FilterKeysConfirmation=True



)";

std::string GetTargetPath() {
    const char* userProfile = std::getenv("USERPROFILE");
    if (!userProfile) {
        std::cerr << "USERPROFILE nicht gefunden\n";
        exit(1);
    }
    std::filesystem::path target = std::filesystem::path(userProfile)
        / "Documents" / "My Games" / "Oblivion Remastered" / "Saved" / "Config" / "Windows";
    std::filesystem::create_directories(target);
    return (target / "Engine.ini").string();
}
// 1) Ermittelt den dedizierten VRAM und berechnet eine PoolSize (95%)
double detectPoolSizeMB()
{
    IDXGIFactory* pFactory = nullptr;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory))))
    {
        std::cerr << "Fehler beim Erstellen der DXGI Factory" << std::endl;
        return 4096.0; // Fallback
    }

    IDXGIAdapter* pBestAdapter = nullptr;
    DXGI_ADAPTER_DESC bestDesc = {};
    UINT idx = 0;
    IDXGIAdapter* pAdapter = nullptr;

    while (pFactory->EnumAdapters(idx, &pAdapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC desc;
        if (SUCCEEDED(pAdapter->GetDesc(&desc)))
        {
            if (!pBestAdapter || desc.DedicatedVideoMemory > bestDesc.DedicatedVideoMemory)
            {
                if (pBestAdapter) pBestAdapter->Release();
                pBestAdapter = pAdapter;
                bestDesc = desc;
            }
            else
            {
                pAdapter->Release();
            }
        }
        else
        {
            pAdapter->Release();
        }
        ++idx;
    }

    double poolSize = 4096.0;
    if (pBestAdapter)
    {
        poolSize = (bestDesc.DedicatedVideoMemory / (1024.0 * 1024.0)) * 0.95;
        pBestAdapter->Release();
    }

    pFactory->Release();
    return poolSize;
}

// 2) Rundet den PoolSize-Wert und ersetzt den Marker im Template
std::string generateConfig(double rawPoolSize)
{
    int rounded = static_cast<int>(std::round(rawPoolSize));
    std::string cfg = configTemplate;
    size_t pos = cfg.find("{POOLSIZE}");
    if (pos != std::string::npos)
    {
        cfg.replace(pos, std::string("{POOLSIZE}").length(), std::to_string(rounded));
    }
    return cfg;
}

int main() {
    int poolSize = detectPoolSizeMB();
    std::cout << "Berechneter PoolSize: " << poolSize << " MB\n";

    // {POOLSIZE} in beiden Templates ersetzen
    std::string part1 = std::regex_replace(configTemplate, std::regex(R"(\{POOLSIZE\})"), std::to_string(poolSize));
    std::string part2 = std::regex_replace(configTemplate2, std::regex(R"(\{POOLSIZE\})"), std::to_string(poolSize));

    // Zielpfad holen
    std::string targetPath = GetTargetPath();

    // Datei löschen, falls vorhanden
    // 
// Backup, falls vorhanden
    if (std::filesystem::exists(targetPath)) {
        std::filesystem::path backupPath = targetPath;
        backupPath.replace_filename("Engine_backup.ini");
        std::error_code ec;
        std::filesystem::copy_file(targetPath, backupPath, std::filesystem::copy_options::overwrite_existing, ec);
        if (ec) {
            std::cerr << "Fehler beim Erstellen des Backups: " << ec.message() << "\n";
        }
        else {
            std::cout << "Backup erstellt unter: " << backupPath << "\n";
        }
        std::remove(targetPath.c_str()); // jetzt löschen
    }

    // Datei neu schreiben mit kombiniertem Inhalt
    std::ofstream outFile(targetPath);
    if (!outFile) {
        std::cerr << "Konnte Datei nicht schreiben: " << targetPath << "\n";
        return 1;
    }

    outFile << part1 << "\n" << part2;
    outFile.close();

    std::cout << "INI-Datei erfolgreich neu erstellt unter:\n" << targetPath << "\n";
    return 0;
}