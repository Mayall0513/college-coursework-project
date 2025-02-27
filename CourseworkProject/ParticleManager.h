#pragma once

#include "stdafx.h"

#include "Engine.h"

#include <list>
#include <random>

#include <memory>
#include <iostream>

namespace particles {
	class ParticleEmitter {
	private:
		struct Particle {
		public:
			std::unique_ptr<sf::Vertex[]> vertices = std::make_unique<sf::Vertex[]>(4U);
			sf::Vector2f location;
			sf::Vector2f velocity;

			float resistance;
			float fullLifeSeconds;
			float lifeSeconds;
			float rotationsPerSecond;

			unsigned char colorAlpha;

			bool fadesIn;
			float fadeInEndSeconds;

			bool fadesOut;
			float fadeOutStartSeconds;
			float fadeOutLengthSeconds;

			Particle(const ParticleEmitter * parentEmitter);

			bool update(float deltaSeconds); //returns true if the particle has finished its life time
		};

		unsigned int maxTotalParticles = 0U;
		unsigned int totalParticles = 0U;
		unsigned int maxWorldParticles = 20U;

		std::list<Particle> particles = std::list<Particle>();
		bool particleUseRandomDirection = true;
		float particleSpeed = 1.F;
		float particleResistance = 0.F;
		float particleEmitTimeSeconds = 0.5F;
		float particleRotationsPerSecond = 0.F;
		float particleLifeSeconds = 10.F;
		float particleDirection = 0.F;
		sf::Color particleColor = sf::Color::White;
		sf::Vector2f particleSize = sf::Vector2f(10.F, 10.F);
		sf::Vector2f particleStartVelocity;
		sf::Vector2f particleStartVelocityMultiplied;
		std::unique_ptr<sf::Vertex[]> particleOriginalVertices = std::make_unique<sf::Vertex[]>(4U);

		bool usingMultipleTextures = false;
		unsigned int particleTextureCount = 0U;
		sf::Vector2f * particleTextureCoordinates = nullptr;
		std::uniform_int_distribution<unsigned int> textureDistribution = std::uniform_int_distribution<unsigned int>(0U);

		bool fadesIn = true;
		float fadeInEnd = 0.1F;
		float fadeInEndSeconds = 1.F;

		bool fadesOut = true;
		float fadeOutStart = 0.5F;
		float fadeOutStartSeconds = 5.F;
		float fadeOutLengthSeconds = 5.F;

		bool usingSpeedVariation = false;
		float speedVariation = 0.F;
		std::uniform_real_distribution<float> speedDistribution = std::uniform_real_distribution<float>(0.F, 0.F);

		bool usingResistanceVariation = false;
		float resistanceVariation = 0.F;
		std::uniform_real_distribution<float> resistanceDistribution = std::uniform_real_distribution<float>(0.F, 0.F);

		bool usingEmitTimeVariation = false;
		float emitTimeVariation = 0.F;
		std::uniform_real_distribution<float> emitTimeDistribution = std::uniform_real_distribution<float>(0.F, 0.F);
		float deviatedParticleEmitTimeSeconds = 0.F;

		bool usingRotationSpeedVariation = false;
		float rotationSpeedVariation = 0.F;
		std::uniform_real_distribution<float> rotationSpeedDistribution = std::uniform_real_distribution<float>(0.F, 0.F);

		bool usingLifeTimeVariation = false;
		float lifeTimeVariation = 0.F;
		std::uniform_real_distribution<float> lifeTimeDistribution = std::uniform_real_distribution<float>(0.F, 0.F);

		bool usingDirectionVariation = false;
		float directionVariation = 0.F;
		std::uniform_real_distribution<float> directionDistribution = std::uniform_real_distribution<float>(0.F, 0.F);

		bool usingSizeVariation = false;
		float sizeVariation = 0.F;
		std::uniform_real_distribution<float> sizeDistribution = std::uniform_real_distribution<float>(1.F, 1.F);

		bool emitterVertexCountChanged = false;
		unsigned int emitterVertexCount = 0U;
		float emitterDelta = 0.F;
		bool emitterFinished = false;
		sf::Vector2f emitterLocation = sf::Vector2f();
		std::unique_ptr<sf::Vertex[]> emitterParticleVertices = std::make_unique<sf::Vertex[]>(120U);

	public:
		ParticleEmitter();

		void update(float deltaSeconds);
		void zoomEmitter(float scaleFactor);

		inline unsigned int getMaxTotalParticles() const { return maxTotalParticles; }
		inline unsigned int getTotalParticles() const { return totalParticles; }
		inline unsigned int getMaxWorldParticles() const { return maxWorldParticles; }
		inline std::size_t getWorldParticles() const { return particles.size(); }

		inline bool getParticleUseRandomDirection() const { return particleUseRandomDirection; }
		inline float getParticleSpeed() const { return particleSpeed; }
		inline float getParticleResistance() const { return particleResistance; }
		inline float getParticleEmitTimeSeconds() const { return particleEmitTimeSeconds; }
		inline float getParticleRotationsPerSecond() const { return particleRotationsPerSecond; }
		inline float getParticleLifeSeconds() const { return particleLifeSeconds; }
		inline float getParticleDirection() const { return particleDirection; }
		inline sf::Color getParticleColor() const { return particleColor; }
		inline sf::Vector2f getParticleSize() const { return particleSize; }

		inline sf::Vector2f * getParticleTextureCoordinates() const { return particleTextureCoordinates; }
		inline unsigned int getParticleTextureCount() const { return particleTextureCount; }

		inline bool getFadesIn() const { return fadesIn; }
		inline float getFadeInEnd() const { return fadeInEnd; }

		inline bool getFadesOut() const { return fadesOut; }
		inline float getFadeOutStart() const { return fadeOutStart; }

		inline bool getUsingSpeedVariation() const { return usingSpeedVariation; }
		inline float getSpeedVariation() const { return speedVariation; }

		inline bool getUsingResistanceVariation() const { return usingResistanceVariation; }
		inline float getResistanceVariation() const { return resistanceVariation; }

		inline bool getUsingEmitTimeVariation() const { return usingEmitTimeVariation; }
		inline float getEmitTimeVariation() const { return emitTimeVariation; }

		inline bool getUsingRotationSpeedVariation() const { return usingRotationSpeedVariation; }
		inline float getRotationSpeedVariation() const { return rotationSpeedVariation; }

		inline bool getUsingLifeTimeVariation() const { return usingLifeTimeVariation; }
		inline float getLifeTimeVariation() const { return lifeTimeVariation; }

		inline bool getUsingDirectionVariation() const { return usingDirectionVariation; }
		inline float getDirectionVariation() const { return directionVariation; }

		inline bool getUsingSizeVariation() const { return usingSizeVariation; }
		inline float getSizeVariation() const { return sizeVariation; }

		inline bool getEmitterVertexCountChanged() const { return emitterVertexCountChanged; }
		inline unsigned int getEmitterVertexCount() const { return emitterVertexCount; }
		inline bool getEmitterFinished() const { return emitterFinished; }
		inline sf::Vector2f getEmitterLocation() const { return emitterLocation; }
		inline sf::Vertex * getEmitterParticleVertices() const { return emitterParticleVertices.get(); }

		inline void setMaxTotalParticles(unsigned int totalParticles) { maxTotalParticles = totalParticles; }
		void setMaxWorldParticles(unsigned int worldParticles);

		inline void setParticleUseRandomDirection(bool useRandomDirection) { particleUseRandomDirection = useRandomDirection; }
		void setParticleSpeed(float speed);
		void setParticleResistance(float resistance);
		void setParticleEmitTimeSeconds(float emitTimeSeconds);
		void setParticleRotationsPerSecond(float rotationsPerSecond);
		void setParticleLifeSeconds(float lifeSeconds);
		void setParticleDirection(float direction);
		void setParticleColor(const sf::Color & color);
		void setParticleSize(const sf::Vector2f & size);

		void setUsingMultipleTextures(bool useMultipleTextures);
		void setParticleTextureCoordinates(sf::Vector2f * const textureCoordinates, unsigned int textureCount);

		inline void setFadesIn(bool fadesIn) { this->fadesIn = fadesIn; }
		void setFadeInEnd(float fadeInEnd);

		inline void setFadesOut(bool fadesOut) { this->fadesOut = fadesOut; }
		void setFadeOutStart(float fadeOutStart);

		inline void setUsingSpeedVariation(bool useSpeedVariation) { usingSpeedVariation = useSpeedVariation; }
		void setSpeedVariation(float speedVariation);

		inline void setUsingResistanceVariation(bool useResistanceVariation) { usingResistanceVariation = useResistanceVariation; }
		void setResistanceVariation(float resistanceVariation);

		inline void setUsingEmitTimeVariation(bool useEmitTimeVariation) { usingEmitTimeVariation = useEmitTimeVariation; }
		void setEmitTimeVariation(float emitTimeVariation);

		inline void setUsingRotationSpeedVariation(bool useRotationSpeedVariation) { usingRotationSpeedVariation = useRotationSpeedVariation; }
		void setRotationSpeedVariation(float rotationSpeedVariation);

		inline void setUsingLifeTimeVariation(bool useLifeTimeVariation) { usingLifeTimeVariation = useLifeTimeVariation; }
		void setLifeTimeVariation(float lifeTimeVariation);

		inline void setUsingDirectionVariation(bool useDirectionVariation) { usingDirectionVariation = useDirectionVariation; }
		void setDirectionVariation(float directionVariation);

		inline void setUsingSizeVariation(bool useSizeVariation) { usingSizeVariation = useSizeVariation; }
		void setSizeVariation(float sizeVariation);

		inline void setEmitterLocation(const sf::Vector2f & location) { emitterLocation = location; }

	private:
		void recalculateVertices();
	};

	class ParticleManager : public sf::Drawable {
	private:
		std::list<ParticleEmitter> particleEmitters;
		sf::VertexBuffer particleVertexBuffer;

	public:
		engine::CombinedRenderStates managerRenderStates;

		ParticleManager();

		void update(float deltaSeconds);
		inline void draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const { renderTarget.draw(particleVertexBuffer, managerRenderStates); }
		void zoomManager(float scaleFactor);

		ParticleEmitter & addEmitter(bool atBack = false);
		void clearEmitters();

	private:
		void recollectVertices();
	};
};

