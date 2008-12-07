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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Shader.h"

#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include <OgreSceneManager.h>
#include <OgreRoot.h>
#include <OgrePlugin.h>

#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "GraphicsEngine.h"
#include "util/Exception.h"

#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreMaterialManager.h>

namespace orxonox
{
    bool Shader::bLoadedCgPlugin_s = false;
    Shader::MaterialMap Shader::parameters_s;

    Shader::Shader(Ogre::SceneManager* scenemanager)
    {
        RegisterObject(Shader);

        this->scenemanager_ = scenemanager;
        this->compositorInstance_ = 0;
        this->bVisible_ = true;
        this->bLoadCompositor_ = Core::showsGraphics() && GraphicsEngine::getInstancePtr();
        this->bViewportInitialized_ = false;
        this->compositor_ = "";
        this->oldcompositor_ = "";
//        this->parameter_ = 0;

        if (this->bLoadCompositor_ && Ogre::Root::getSingletonPtr())
        {
            Shader::bLoadedCgPlugin_s = false;
            const Ogre::Root::PluginInstanceList& plugins = Ogre::Root::getSingleton().getInstalledPlugins();
            for (size_t i = 0; i < plugins.size(); ++i)
            {
                if (plugins[i]->getName() == "Cg Program Manager")
                {
                    Shader::bLoadedCgPlugin_s = true;
                    break;
                }
            }
        }

        this->bLoadCompositor_ &= Shader::bLoadedCgPlugin_s;
    }

    Shader::~Shader()
    {
/*
        if (this->isInitialized() && this->bLoadCompositor_ && this->compositor_ != "")
        {
            Ogre::Viewport* viewport = GraphicsEngine::getInstance().getViewport();
            assert(viewport);
            Ogre::CompositorManager::getSingleton().removeCompositor(viewport, this->compositor_);
        }
*/
    }

    void Shader::setSceneManager(Ogre::SceneManager* scenemanager)
    {
        this->scenemanager_ = scenemanager;
        this->bViewportInitialized_ = false;
    }

    void Shader::tick(float dt)
    {
        if (this->bLoadCompositor_ && !this->bViewportInitialized_ && this->scenemanager_ && this->scenemanager_->getCurrentViewport())
        {
            this->bViewportInitialized_ = true;
            this->updateVisibility();
        }

/*
        COUT(0) << std::endl << "GpuPrograms:" << std::endl;
        Ogre::GpuProgramManager::ResourceMapIterator iterator = Ogre::GpuProgramManager::getSingleton().getResourceIterator();
        while (iterator.hasMoreElements())
            COUT(0) << iterator.getNext()->getName() << std::endl;
*/
/*
        Ogre::Resource* resource = Ogre::GpuProgramManager::getSingleton().getByName("Radial_Blur_ps_cg").get();
        if (resource)
        {
            COUT(0) << std::endl << "Resource parameters:" << std::endl;
            const Ogre::ParameterList& resourceparameters = resource->getParameters();
            for (size_t i = 0; i < resourceparameters.size(); ++i)
                COUT(0) << resourceparameters[i].name << std::endl;

            COUT(0) << std::endl << "GpuProgram:" << std::endl;
            Ogre::GpuProgram* gpuprogram = (Ogre::GpuProgram*)resource;
            COUT(0) << gpuprogram << std::endl;

            COUT(0) << std::endl << "GpuProgram default parameters:" << std::endl;
            Ogre::GpuProgramParameters* defaultparams = gpuprogram->getDefaultParameters().get();
            COUT(0) << defaultparams << std::endl;
            if (defaultparams)
            {
//                defaultparams->setNamedConstant("sampleStrength", 2.0f);

                const Ogre::GpuConstantDefinitionMap& constantdefinitions = defaultparams->getConstantDefinitions().map;
                for (Ogre::GpuConstantDefinitionMap::const_iterator it = constantdefinitions.begin(); it != constantdefinitions.end(); ++it)
                    COUT(0) << it->first << " / " << it->second.physicalIndex << " / " << it->second.isFloat() << " / " << (float)((it->second.isFloat()) ? (*defaultparams->getFloatPointer(it->second.physicalIndex)) : (*defaultparams->getIntPointer(it->second.physicalIndex))) << std::endl;

                Ogre::GpuConstantDefinitionMap::const_iterator it = constantdefinitions.find("sampleStrength");
                if (it != constantdefinitions.end())
                {
                    if (it->second.isFloat())
                    {
                        COUT(0) << (defaultparams->getFloatPointer(it->second.physicalIndex)) << std::endl;
                        COUT(0) << (*defaultparams->getFloatPointer(it->second.physicalIndex)) << std::endl;
                        (*defaultparams->getFloatPointer(it->second.physicalIndex)) = 10.0f;
                    }
                    else
                    {
                        COUT(0) << (defaultparams->getIntPointer(it->second.physicalIndex)) << std::endl;
                        COUT(0) << (*defaultparams->getIntPointer(it->second.physicalIndex)) << std::endl;
                        (*defaultparams->getIntPointer(it->second.physicalIndex)) = 10;
                    }
                    // doesn't work because it's "manually loaded":
                    //
                    // Ogre: Creating viewport on target 'rtt/166241248', rendering from camera '140', relative
                    // dimensions L: 0.00 T: 0.00 W: 1.00 H: 1.00 ZOrder: 0
                    //
                    // Ogre: WARNING: Texture instance 'CompositorInstanceTexture0' was defined as manually
                    // loaded, but no manual loader was provided. This Resource will be lost if it has to be
                    // reloaded.
                    //
//                    gpuprogram->reload();
                }

            }

            COUT(0) << "end" << std::endl;
        }
*/
/*
        if (this->parameter_ && this->bViewportInitialized_)
        {
            (*this->parameter_) += dt;

            if (this->compositorInstance_ && this->isVisible())
            {
                this->compositorInstance_->setEnabled(false);
                this->compositorInstance_->setEnabled(true);
            }
        }
*/

//        this->setParameter("Ogre/Compositor/Radial_Blur", 0, 0, "sampleStrength", Shader::getParameter("Ogre/Compositor/Radial_Blur", 0, 0, "sampleStrength") + dt / 3.0f);
//        this->setParameter("Ogre/Compositor/Tiling", 0, 0, "NumTiles", Shader::getParameter("Ogre/Compositor/Tiling", 0, 0, "NumTiles") + dt);
    }

    void Shader::changedCompositor()
    {
        if (this->bLoadCompositor_)
        {
            Ogre::Viewport* viewport = GraphicsEngine::getInstance().getViewport();
            assert(viewport);
            if (this->oldcompositor_ != "")
            {
                Ogre::CompositorManager::getSingleton().removeCompositor(viewport, this->oldcompositor_);
                this->compositorInstance_ = 0;
            }
            if (this->compositor_ != "")
            {
                this->compositorInstance_ = Ogre::CompositorManager::getSingleton().addCompositor(viewport, this->compositor_);
                if (!this->compositorInstance_)
                    COUT(2) << "Warning: Couldn't load compositor with name \"" << this->compositor_ << "\"." << std::endl;
                else
                {
/*
                    COUT(0) << std::endl << "GpuPrograms:" << std::endl;
                    Ogre::GpuProgramManager::ResourceMapIterator gpuprogramiterator = Ogre::GpuProgramManager::getSingleton().getResourceIterator();
                    while (gpuprogramiterator.hasMoreElements())
                        COUT(0) << gpuprogramiterator.getNext()->getName() << std::endl;
*/
/*
                    COUT(0) << std::endl << "compositor:" << std::endl;
                    Ogre::Compositor* compositor = this->compositorInstance_->getCompositor();
                    COUT(0) << compositor << std::endl;

                    if (!compositor)
                        return;

                    COUT(0) << std::endl << "compositor parameters:" << std::endl;
                    const Ogre::ParameterList& compparameters = compositor->getParameters();
                    for (size_t i = 0; i < compparameters.size(); ++i)
                        COUT(0) << compparameters[i].name << std::endl;

                    COUT(0) << std::endl << "compositor technique:" << std::endl;
                    Ogre::CompositionTechnique* comptechnique = compositor->getSupportedTechnique(0);
                    COUT(0) << comptechnique << std::endl;

                    if (!comptechnique)
                        return;

                    COUT(0) << std::endl << "compositor target pass:" << std::endl;
                    Ogre::CompositionTargetPass* comptargetpass = comptechnique->getTargetPass(0);
                    COUT(0) << comptargetpass << std::endl;

                    if (!comptargetpass)
                        return;

                    COUT(0) << std::endl << "compositor pass:" << std::endl;
                    Ogre::CompositionPass* comppass = comptargetpass->getPass(0);
                    COUT(0) << comppass << std::endl;

                    if (!comppass)
                        return;

                    COUT(0) << std::endl << "material:" << std::endl;
                    Ogre::Material* material = comppass->getMaterial().get();
                    COUT(0) << material << std::endl;
*/
/*
                    COUT(0) << std::endl << "Materials:" << std::endl;
                    Ogre::MaterialManager::ResourceMapIterator iterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
                    Ogre::Material* material = 0;
                    while (iterator.hasMoreElements())
                    {
                        Ogre::Resource* resource = iterator.getNext().get();
                        COUT(0) << resource->getName() << std::endl;
                        if (resource->getName() == "Ogre/Compositor/Radial_Blur")
                        {
                            material = (Ogre::Material*)resource;
                            COUT(0) << "found it!" << std::endl;
                        }
                    }

                    if (!material)
                        return;

                    COUT(0) << std::endl << "material parameters:" << std::endl;
                    const Ogre::ParameterList& materialparameters = material->getParameters();
                    for (size_t i = 0; i < materialparameters.size(); ++i)
                        COUT(0) << materialparameters[i].name << std::endl;

                    COUT(0) << std::endl << "technique:" << std::endl;
                    Ogre::Technique* technique = material->getTechnique(0);
                    COUT(0) << technique << std::endl;

                    if (!technique)
                        return;

                    COUT(0) << std::endl << "pass:" << std::endl;
                    Ogre::Pass* pass = technique->getPass(0);
                    COUT(0) << pass << std::endl;

                    if (!pass)
                        return;

                    COUT(0) << std::endl << "fragment program parameters:" << std::endl;
                    Ogre::GpuProgramParameters* fragmentparams = pass->getFragmentProgramParameters().get();
                    COUT(0) << fragmentparams << std::endl;
                    if (fragmentparams)
                    {
                        const Ogre::GpuConstantDefinitionMap& constantdefinitions = fragmentparams->getConstantDefinitions().map;
                        for (Ogre::GpuConstantDefinitionMap::const_iterator it = constantdefinitions.begin(); it != constantdefinitions.end(); ++it)
                            COUT(0) << it->first << " / " << it->second.physicalIndex << " / " << it->second.isFloat() << " / " << (float)((it->second.isFloat()) ? (*fragmentparams->getFloatPointer(it->second.physicalIndex)) : (*fragmentparams->getIntPointer(it->second.physicalIndex))) << std::endl;

                        Ogre::GpuConstantDefinitionMap::const_iterator it = constantdefinitions.find("sampleStrength");

                        if (it != constantdefinitions.end())
                        {
                            if (it->second.isFloat())
                            {
                                COUT(0) << (fragmentparams->getFloatPointer(it->second.physicalIndex)) << std::endl;
                                COUT(0) << (*fragmentparams->getFloatPointer(it->second.physicalIndex)) << std::endl;
                                (*fragmentparams->getFloatPointer(it->second.physicalIndex)) = 0.0f;
                                this->parameter_ = fragmentparams->getFloatPointer(it->second.physicalIndex);
                            }
                            else
                            {
                                COUT(0) << (fragmentparams->getIntPointer(it->second.physicalIndex)) << std::endl;
                                COUT(0) << (*fragmentparams->getIntPointer(it->second.physicalIndex)) << std::endl;
                                (*fragmentparams->getIntPointer(it->second.physicalIndex)) = 0;
                            }
                        }
                    }

                    COUT(0) << std::endl << "fragment program:" << std::endl;
                    COUT(0) << (&pass->getFragmentProgram()) << std::endl;

                    COUT(0) << std::endl << "fragment program name:" << std::endl;
                    COUT(0) << pass->getFragmentProgramName() << std::endl;

                    COUT(0) << std::endl << "GpuProgram default parameters:" << std::endl;
                    Ogre::GpuProgramParameters* defaultparams = pass->getFragmentProgram().get()->getDefaultParameters().get();
                    COUT(0) << defaultparams << std::endl;
                    if (defaultparams)
                    {
                        const Ogre::GpuConstantDefinitionMap& constantdefinitions = defaultparams->getConstantDefinitions().map;
                        for (Ogre::GpuConstantDefinitionMap::const_iterator it = constantdefinitions.begin(); it != constantdefinitions.end(); ++it)
                            COUT(0) << it->first << " / " << it->second.physicalIndex << " / " << it->second.isFloat() << " / " << (float)((it->second.isFloat()) ? (*defaultparams->getFloatPointer(it->second.physicalIndex)) : (*defaultparams->getIntPointer(it->second.physicalIndex))) << std::endl;
                    }

                    COUT(0) << std::endl << "end" << std::endl;
*/
                }
                Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, this->compositor_, this->bViewportInitialized_ && this->isVisible());
            }
            this->oldcompositor_ = this->compositor_;
        }
    }

    void Shader::updateVisibility()
    {
        if (this->compositorInstance_ && this->scenemanager_)
            this->compositorInstance_->setEnabled(this->scenemanager_->getCurrentViewport() && this->isVisible());
    }

    void Shader::setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, float value)
    {
        if (Shader::_setParameter(material, technique, pass, parameter, value))
        {
            if (this->bViewportInitialized_ && this->compositorInstance_ && this->isVisible())
            {
                this->compositorInstance_->setEnabled(false);
                this->compositorInstance_->setEnabled(true);
            }
        }
    }

    void Shader::setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, int value)
    {
        if (Shader::_setParameter(material, technique, pass, parameter, value))
        {
            if (this->bViewportInitialized_ && this->compositorInstance_ && this->isVisible())
            {
                this->compositorInstance_->setEnabled(false);
                this->compositorInstance_->setEnabled(true);
            }
        }
    }

    bool Shader::_setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, float value)
    {
        ParameterPointer* pointer = Shader::getParameterPointer(material, technique, pass, parameter);
        if (pointer)
        {
            if (pointer->first)
            {
                if ((*((float*)pointer->second)) != value)
                {
                    (*((float*)pointer->second)) = value;
                    return true;
                }
            }
            else
            {
                if ((*((int*)pointer->second)) != (int)value)
                {
                    (*((int*)pointer->second)) = (int)value;
                    return true;
                }
            }
        }
        return false;
    }

    bool Shader::_setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, int value)
    {
        ParameterPointer* pointer = Shader::getParameterPointer(material, technique, pass, parameter);
        if (pointer)
        {
            if (pointer->first)
            {
                if ((*((float*)pointer->second)) != (float)value)
                {
                    (*((float*)pointer->second)) = (float)value;
                    return true;
                }
            }
            else
            {
                if ((*((int*)pointer->second)) != value)
                {
                    (*((int*)pointer->second)) = value;
                    return true;
                }
            }
        }
        return false;
    }

    float Shader::getParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter)
    {
        ParameterPointer* pointer = Shader::getParameterPointer(material, technique, pass, parameter);
        if (pointer)
        {
            if (pointer->first)
                return (*((float*)pointer->second));
            else
                return (*((int*)pointer->second));
        }
        else
            return 0;
    }

    bool Shader::getParameterIsFloat(const std::string& material, size_t technique, size_t pass, const std::string& parameter)
    {
        ParameterPointer* pointer = Shader::getParameterPointer(material, technique, pass, parameter);
        if (pointer)
            return pointer->first;
        else
            return false;
    }

    bool Shader::getParameterIsInt(const std::string& material, size_t technique, size_t pass, const std::string& parameter)
    {
        ParameterPointer* pointer = Shader::getParameterPointer(material, technique, pass, parameter);
        if (pointer)
            return (!pointer->first);
        else
            return false;
    }

    Shader::ParameterPointer* Shader::getParameterPointer(const std::string& material, size_t technique, size_t pass, const std::string& parameter)
    {
        if (!Core::showsGraphics() || !Shader::bLoadedCgPlugin_s)
            return 0;

        MaterialMap::iterator material_iterator = Shader::parameters_s.find(material);
        if (material_iterator != Shader::parameters_s.end())
        {
            TechniqueVector& technique_vector = material_iterator->second;
            if (technique < technique_vector.size())
            {
                PassVector& pass_vector = technique_vector[technique];
                if (pass < pass_vector.size())
                {
                    ParameterMap& parameter_map = pass_vector[pass];
                    ParameterMap::iterator parameter_iterator = parameter_map.find(parameter);

                    if (parameter_iterator != parameter_map.end())
                        return (&parameter_iterator->second);
                    else
                        COUT(2) << "Warning: No shader parameter \"" << parameter << "\" in pass " << pass << " in technique " << technique << " in material \"" << material << "\"." << std::endl;
                }
                else
                    COUT(2) << "Warning: No pass " << pass << " in technique " << technique << " in material \"" << material << "\" or pass has no shader." << std::endl;
            }
            else
                COUT(2) << "Warning: No technique " << technique << " in material \"" << material << "\" or technique has no pass with shader." << std::endl;
        }
        else
        {
            bool foundAtLeastOneShaderParameter = false;
            Ogre::MaterialManager::ResourceMapIterator iterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
            Ogre::Material* material_pointer = 0;

            while (iterator.hasMoreElements())
            {
                Ogre::Resource* resource = iterator.getNext().get();
                if (resource->getName() == material)
                    material_pointer = (Ogre::Material*)resource;
            }

            if (!material_pointer)
            {
                COUT(2) << "Warning: No material with name \"" << material << "\" found." << std::endl;
                return 0;
            }

            for (unsigned int t = 0; t < material_pointer->getNumTechniques(); ++t)
            {
                Ogre::Technique* technique_pointer = material_pointer->getTechnique(t);
                if (!technique_pointer)
                    continue;

                for (unsigned int p = 0; p < technique_pointer->getNumPasses(); ++p)
                {
                    Ogre::Pass* pass_pointer = technique_pointer->getPass(p);
                    if (!pass_pointer)
                        continue;

                    if (pass_pointer->getFragmentProgramName() != "")
                    {
                        Ogre::GpuProgramParameters* parameter_pointer = pass_pointer->getFragmentProgramParameters().get();
                        if (!parameter_pointer)
                            continue;

                        const Ogre::GpuConstantDefinitionMap& constant_definitions = parameter_pointer->getConstantDefinitions().map;
                        for (Ogre::GpuConstantDefinitionMap::const_iterator definition_iterator = constant_definitions.begin(); definition_iterator != constant_definitions.end(); ++definition_iterator)
                        {
                            void* temp = (definition_iterator->second.isFloat())
                                            ? (void*)parameter_pointer->getFloatPointer(definition_iterator->second.physicalIndex)
                                            : (void*)parameter_pointer->getIntPointer(definition_iterator->second.physicalIndex);
                            ParameterPointer parameter_pointer = ParameterPointer(definition_iterator->second.isFloat(), temp);

                            TechniqueVector& technique_vector = Shader::parameters_s[material];
                            technique_vector.resize(technique + 1);
                            PassVector& pass_vector = technique_vector[technique];
                            pass_vector.resize(pass + 1);
                            pass_vector[pass][definition_iterator->first] = parameter_pointer;
                            foundAtLeastOneShaderParameter = true;
                        }
                    }
                }
            }

            // recursive call if the material was added to the map
            if (foundAtLeastOneShaderParameter)
                return Shader::getParameterPointer(material, technique, pass, parameter);
        }
        return 0;
    }
}
