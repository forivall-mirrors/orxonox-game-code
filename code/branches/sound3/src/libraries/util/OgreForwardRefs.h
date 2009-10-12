/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _OgreForwardRefs_H__
#define _OgreForwardRefs_H__

#include "UtilPrereqs.h"

namespace Ogre
{
    class Angle;
    class Animation;
    class AnimationState;
    class AnimationStateSet;
    class AnimationTrack;
    class Archive;
    class ArchiveFactory;
    class ArchiveManager;
    class AutoParamDataSource;
    class AxisAlignedBox;
    class AxisAlignedBoxSceneQuery;
    class Billboard;
    class BillboardChain;
    class BillboardSet;
    class Bone;
    class Camera;
    class Codec;
    class ColourValue;
    class ConfigDialog;
    template <typename T> class Controller;
    template <typename T> class ControllerFunction;
    class ControllerManager;
    template <typename T> class ControllerValue;
    class Degree;
    class DynLib;
    class DynLibManager;
    class EdgeData;
    class EdgeListBuilder;
    class Entity;
    class ErrorDialog;
    class ExternalTextureSourceManager;
    class Factory;
    class Font;
    class FontPtr;
    class FontManager;
    struct FrameEvent;
    class FrameListener;
    class Frustum;
    class GpuProgram;
    class GpuProgramPtr;
    class GpuProgramManager;
	class GpuProgramUsage;
    class HardwareIndexBuffer;
    class HardwareOcclusionQuery;
    class HardwareVertexBuffer;
	class HardwarePixelBuffer;
    class HardwarePixelBufferSharedPtr;
	class HighLevelGpuProgram;
    class HighLevelGpuProgramPtr;
	class HighLevelGpuProgramManager;
	class HighLevelGpuProgramFactory;
    class IndexData;
    class IntersectionSceneQuery;
    class IntersectionSceneQueryListener;
    class Image;
    class KeyFrame;
    class Light;
    class Log;
    class LogManager;
	class ManualResourceLoader;
	class ManualObject;
    class Material;
    class MaterialPtr;
    class MaterialManager;
    class MaterialScriptCompiler;
    class Math;
    class Matrix3;
    class Matrix4;
    class MemoryManager;
    class Mesh;
    class MeshPtr;
    class MeshSerializer;
    class MeshSerializerImpl;
    class MeshManager;
    class MovableObject;
    class MovablePlane;
    class Node;
	class NodeAnimationTrack;
	class NodeKeyFrame;
	class NumericAnimationTrack;
	class NumericKeyFrame;
    class Overlay;
    class OverlayContainer;
    class OverlayElement;
    class OverlayElementFactory;
    class OverlayManager;
    class Particle;
    class ParticleAffector;
    class ParticleAffectorFactory;
    class ParticleEmitter;
    class ParticleEmitterFactory;
    class ParticleSystem;
    class ParticleSystemManager;
    class ParticleSystemRenderer;
    class ParticleSystemRendererFactory;
    class ParticleVisualData;
    class Pass;
    class PatchMesh;
    class PixelBox;
    class Plane;
    class PlaneBoundedVolume;
	class Plugin;
    class Pose;
    class ProgressiveMesh;
    class Profile;
	class Profiler;
    class Quaternion;
	class Radian;
    class Ray;
    class RaySceneQuery;
    class RaySceneQueryListener;
    class Renderable;
    class RenderPriorityGroup;
    class RenderQueue;
    class RenderQueueGroup;
	class RenderQueueInvocation;
	class RenderQueueInvocationSequence;
    class RenderQueueListener;
    class RenderSystem;
    class RenderSystemCapabilities;
    class RenderTarget;
    class RenderTargetListener;
    class RenderTexture;
	class MultiRenderTarget;
    class RenderWindow;
    class RenderOperation;
    class Resource;
	class ResourceBackgroundQueue;
	class ResourceGroupManager;
    class ResourceManager;
    class RibbonTrail;
	class Root;
    class SceneManager;
    class SceneManagerEnumerator;
    class SceneNode;
    class SceneQuery;
    class SceneQueryListener;
	class ScriptLoader;
    class Serializer;
    class ShadowCaster;
    class ShadowRenderable;
	class ShadowTextureManager;
    class SimpleRenderable;
    class SimpleSpline;
    class Skeleton;
    class SkeletonPtr;
    class SkeletonInstance;
    class SkeletonManager;
    class Sphere;
    class SphereSceneQuery;
	class StaticGeometry;
    class StringConverter;
    class StringInterface;
    class SubEntity;
    class SubMesh;
	class TagPoint;
    class Technique;
	class TempBlendedBufferInfo;
	class ExternalTextureSource;
    class TextureUnitState;
    class Texture;
    class TexturePtr;
	class TextureFont;
    class TextureManager;
    class TransformKeyFrame;
	class Timer;
    class UserDefinedObject;
    class Vector2;
    class Vector3;
    class Vector4;
    class Viewport;
	class VertexAnimationTrack;
    class VertexBufferBinding;
    class VertexData;
    class VertexDeclaration;
	class VertexMorphKeyFrame;
    class WireBoundingBox;
    class Compositor;
    class CompositorManager;
    class CompositorChain;
    class CompositorInstance;
    class CompositionTechnique;
    class CompositionPass;
    class CompositionTargetPass;

    // Originally not in OgrePrerequisites.h
    class BorderPanelOverlayElement;
    class PanelOverlayElement;
    class TextAreaOverlayElement;
}

#endif /* _OgreForwardRefs_H__ */
