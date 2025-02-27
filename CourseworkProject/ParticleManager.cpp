#include "ParticleManager.h"

#include "GameResources.h"
#include "Engine.h"

particles::ParticleEmitter::Particle::Particle(const ParticleEmitter * parentEmitter) {
	//Initialze the particle and apply and random variation the emitter specifies.
	location = parentEmitter->emitterLocation;

	resistance = parentEmitter->particleResistance;
	if (parentEmitter->usingResistanceVariation) {
		resistance += parentEmitter->resistanceDistribution(resources::GAME_RANDOMENGINE);
	}

	fullLifeSeconds = parentEmitter->particleLifeSeconds;
	if (parentEmitter->usingLifeTimeVariation) {
		fullLifeSeconds += parentEmitter->lifeTimeDistribution(resources::GAME_RANDOMENGINE);
	}

	lifeSeconds = 0.F;
	rotationsPerSecond = parentEmitter->particleRotationsPerSecond;
	if (parentEmitter->usingRotationSpeedVariation) {
		rotationsPerSecond += parentEmitter->rotationSpeedDistribution(resources::GAME_RANDOMENGINE);
	}

	colorAlpha = parentEmitter->particleColor.a;

	fadesIn = parentEmitter->fadesIn;
	if (fadesIn && parentEmitter->usingLifeTimeVariation) {
		fadeInEndSeconds = parentEmitter->fadeInEnd * fullLifeSeconds;
	}

	else {
		fadeInEndSeconds = parentEmitter->fadeInEndSeconds;
	}

	fadesOut = parentEmitter->fadesOut;
	if (fadesOut && parentEmitter->usingLifeTimeVariation) {
		fadeOutStartSeconds = parentEmitter->fadeOutStart * fullLifeSeconds;
		fadeOutLengthSeconds = fullLifeSeconds - fadeOutStartSeconds;
	}

	else {
		fadeOutStartSeconds = parentEmitter->fadeOutStartSeconds;
		fadeOutLengthSeconds = parentEmitter->fadeOutLengthSeconds;
	}

	if (parentEmitter->particleUseRandomDirection) {
		float angle = resources::ZERO_TWOPI_DISTRIBUTION(resources::GAME_RANDOMENGINE);

		if (parentEmitter->usingSpeedVariation) {
			velocity = sf::Vector2f(std::cosf(angle), std::sinf(angle)) * parentEmitter->particleSpeed;
		}

		else {
			velocity = sf::Vector2f(std::cosf(angle), std::sinf(angle)) * (parentEmitter->particleSpeed + parentEmitter->speedDistribution(resources::GAME_RANDOMENGINE));
		}
	}

	else {
		if (parentEmitter->usingDirectionVariation) {
			float angle = parentEmitter->particleDirection + parentEmitter->directionDistribution(resources::GAME_RANDOMENGINE);
			velocity = sf::Vector2f(std::cosf(angle), std::sinf(angle)) * (parentEmitter->particleSpeed + parentEmitter->speedDistribution(resources::GAME_RANDOMENGINE));
		}

		else {
			velocity = parentEmitter->particleStartVelocityMultiplied;
		}
	}

	if (vertices) {
		memcpy(vertices.get(), parentEmitter->particleOriginalVertices.get(), sizeof(sf::Vertex) * 4U);

		if (parentEmitter->usingMultipleTextures) {
			unsigned int texture = parentEmitter->textureDistribution(resources::GAME_RANDOMENGINE);

			for (unsigned int i = 0U; i < 4U; ++i) {
				vertices[i].texCoords = parentEmitter->particleTextureCoordinates[texture * 4U + i];
			}
		}

		if (parentEmitter->usingSizeVariation) {
			float sizeDeviance = parentEmitter->sizeDistribution(resources::GAME_RANDOMENGINE);

			for (unsigned int i = 0U; i < 4U; ++i) {
				vertices[i].position *= sizeDeviance;
			}
		}
	}
}

bool particles::ParticleEmitter::Particle::update(float deltaSeconds) {
	sf::Vertex * verticesPointer = vertices.get();

	if (velocity.x != 0.F || velocity.y != 0.F) {
		location += velocity * deltaSeconds;
	}

	if (resistance != 0.F) {
		velocity *= 1.F - resistance * deltaSeconds;
	}

	if (rotationsPerSecond != 0.F) {
		float rotations = deltaSeconds * rotationsPerSecond;
		float sinAngle = std::sinf(rotations);
		float cosAngle = std::cosf(rotations);

		for (unsigned int i = 0U; i < 4U; ++i) {
			sf::Vertex & vertex = verticesPointer[i];

			//clockwise rotation - flipped the signs on each sine in the matrix
			vertex.position = sf::Vector2f(cosAngle * vertex.position.x + sinAngle * vertex.position.y, -sinAngle * vertex.position.x + cosAngle * vertex.position.y);
		}
	}

	bool fadingIn = fadesIn && lifeSeconds < fadeInEndSeconds;
	bool fadingOut = !fadingIn && fadesOut && lifeSeconds > fadeOutStartSeconds;

	if (fadingIn || fadingOut) {
		unsigned char trueOpacity = fadingIn ? static_cast<unsigned char>(lifeSeconds / fadeInEndSeconds * colorAlpha) : static_cast<unsigned char>((fullLifeSeconds - lifeSeconds) / fadeOutLengthSeconds * colorAlpha);

		for (unsigned int i = 0U; i < 4U; ++i) {
			verticesPointer[i].color.a = trueOpacity;
		}
	}

	lifeSeconds += deltaSeconds;
	return lifeSeconds > fullLifeSeconds;
}

particles::ParticleEmitter::ParticleEmitter() {
	sf::Vector2f halfSize = particleSize / 2.F;

	particleOriginalVertices[0] = sf::Vector2f(-halfSize.x, halfSize.y);
	particleOriginalVertices[1] = halfSize;
	particleOriginalVertices[2] = sf::Vector2f(halfSize.x, -halfSize.y);
	particleOriginalVertices[3] = -halfSize;
}

void particles::ParticleEmitter::update(float deltaSeconds) {
	int particleCountDelta = 0;
	emitterDelta += deltaSeconds;

	while (emitterDelta >= deviatedParticleEmitTimeSeconds && (maxTotalParticles == 0U || totalParticles < maxTotalParticles) && particles.size() < maxWorldParticles) {
		particles.emplace_front(this); //Create new particles to fit requirements such as time since a particle was last emitted.

		++totalParticles;
		++particleCountDelta;

		emitterDelta -= deviatedParticleEmitTimeSeconds;
		deviatedParticleEmitTimeSeconds = particleEmitTimeSeconds;
		if (usingEmitTimeVariation) {
			deviatedParticleEmitTimeSeconds += emitTimeDistribution(resources::GAME_RANDOMENGINE);
		}
	}

	if (!particles.empty()) {//Simulate particles.
		for (std::list<Particle>::iterator particlesIterator = particles.begin(); particlesIterator != particles.end(); ) {
			if (particlesIterator->update(deltaSeconds)) { //Remove particle of its life time is over.
				particlesIterator = particles.erase(particlesIterator);
				--particleCountDelta;
			}

			else {
				++particlesIterator;
			}
		}

		recalculateVertices();
	}

	else {
		if (maxTotalParticles != 0U && totalParticles >= maxTotalParticles) {
			emitterFinished = true;
		}

		emitterVertexCount = 0U;
	}

	if (particleCountDelta != 0) {
		emitterVertexCountChanged = true;
	}
}

void particles::ParticleEmitter::zoomEmitter(float factor) { //Zooms into the emitter - resizing and repositioning everything as necessary.
	sf::Vector2f newEmitterLocation = emitterLocation * factor;
	unsigned int i = 0U;

	for (Particle & particle : particles) {
		sf::Vector2f & location = particle.location;
		sf::Vertex * particleVerticesPointer = particle.vertices.get();

		location = (location - emitterLocation) * factor + newEmitterLocation;
		particle.velocity *= factor;

		for (unsigned int i = 0U; i < 4U; ++i) {
			particleVerticesPointer[i].position *= factor;
		}
	}

	particleSpeed *= factor;
	particleStartVelocityMultiplied *= factor;
	emitterLocation = newEmitterLocation;
	setParticleSize(particleSize * factor);

	recalculateVertices();
}

void particles::ParticleEmitter::setMaxWorldParticles(unsigned int worldParticles) {
	sf::Vertex * newParticleVertices = new sf::Vertex[6U * worldParticles];

	if (newParticleVertices != nullptr && emitterParticleVertices.get() != nullptr) {
		memcpy(newParticleVertices, emitterParticleVertices.get(), sizeof(sf::Vertex) * 6U * std::min(maxWorldParticles, worldParticles));
		emitterParticleVertices.reset(newParticleVertices);
	}

	maxWorldParticles = worldParticles;
}

void particles::ParticleEmitter::setParticleSpeed(float speed) {
	particleSpeed = speed;
	particleStartVelocityMultiplied = particleStartVelocity * particleSpeed;

	float maxSpeedVariation = speed * speedVariation;
	speedDistribution = std::uniform_real_distribution<float>(-maxSpeedVariation, maxSpeedVariation);
}

void particles::ParticleEmitter::setParticleResistance(float resistance) {
	particleResistance = resistance;

	float maxResistanceVariation = resistance * resistanceVariation;
	resistanceDistribution = std::uniform_real_distribution<float>(-maxResistanceVariation, maxResistanceVariation);
}

void particles::ParticleEmitter::setParticleEmitTimeSeconds(float emitTimeSeconds) {
	particleEmitTimeSeconds = emitTimeSeconds;

	float maxEmitTimeVariation = particleEmitTimeSeconds * emitTimeVariation;
	emitTimeDistribution = std::uniform_real_distribution<float>(-maxEmitTimeVariation, maxEmitTimeVariation);
}

void particles::ParticleEmitter::setParticleRotationsPerSecond(float rotationsPerSecond) {
	particleRotationsPerSecond = rotationsPerSecond;

	float maxRotationSpeedVariation = particleRotationsPerSecond * rotationSpeedVariation;
	rotationSpeedDistribution = std::uniform_real_distribution<float>(-maxRotationSpeedVariation, maxRotationSpeedVariation);
}

void particles::ParticleEmitter::setParticleLifeSeconds(float lifeSeconds) {
	particleLifeSeconds = lifeSeconds;
	fadeOutStartSeconds = lifeSeconds * fadeOutStart;
	fadeOutLengthSeconds = lifeSeconds - fadeOutStartSeconds;

	float maxLifeTimeVariation = lifeSeconds * lifeTimeVariation;
	lifeTimeDistribution = std::uniform_real_distribution<float>(-maxLifeTimeVariation, maxLifeTimeVariation);
}

void particles::ParticleEmitter::setParticleDirection(float direction) {
	particleDirection = direction;
	particleStartVelocity = sf::Vector2f(std::cosf(direction), std::sinf(direction));
	particleStartVelocityMultiplied = particleStartVelocity * particleSpeed;

	float maxDirectionVariation = direction * directionVariation;
	directionDistribution = std::uniform_real_distribution<float>(-maxDirectionVariation, maxDirectionVariation);
}

void particles::ParticleEmitter::setParticleColor(const sf::Color & color) {
	particleColor = color;

	for (unsigned int i = 0U; i < 4U; ++i) {
		particleOriginalVertices[i].color = color;
	}
}

void particles::ParticleEmitter::setParticleSize(const sf::Vector2f & size) {
	particleSize = size;
	sf::Vector2f halfSize = particleSize / 2.F;

	particleOriginalVertices[0].position = sf::Vector2f(-halfSize.x, halfSize.y);
	particleOriginalVertices[1].position = halfSize;
	particleOriginalVertices[2].position = sf::Vector2f(halfSize.x, -halfSize.y);
	particleOriginalVertices[3].position = -halfSize;
	sizeDistribution = std::uniform_real_distribution<float>(1.F - sizeVariation, 1.F + sizeVariation);
}

void particles::ParticleEmitter::setUsingMultipleTextures(bool useMultipleTextures) {
	usingMultipleTextures = useMultipleTextures;
}

void particles::ParticleEmitter::setParticleTextureCoordinates(sf::Vector2f * const textureCoordinates, unsigned int textureCount) {
	this->particleTextureCoordinates = textureCoordinates;
	textureDistribution = std::uniform_int_distribution<unsigned int>(0U, textureCount - 1U);

	for (unsigned int i = 0U; i < 4U; ++i) {
		particleOriginalVertices[i].texCoords = textureCoordinates[i];
	}
}

void particles::ParticleEmitter::setFadeInEnd(float fadeInEnd) {
	this->fadeInEnd = fadeInEnd;
	fadeInEndSeconds = particleLifeSeconds * fadeInEnd;

	if (fadeInEnd > fadeOutStart) {
		setFadeOutStart(fadeInEnd);
	}
}

void particles::ParticleEmitter::setFadeOutStart(float fadeOutStart) {
	this->fadeOutStart = fadeOutStart;
	fadeOutStartSeconds = particleLifeSeconds * fadeOutStart;
	fadeOutLengthSeconds = particleLifeSeconds - fadeOutStartSeconds;

	if (fadeInEnd < fadeOutStart) {
		setFadeInEnd(fadeOutStart);
	}
}

void particles::ParticleEmitter::setSpeedVariation(float speedVariation) {
	this->speedVariation = speedVariation;

	float maxSpeedVariation = particleSpeed * speedVariation;
	speedDistribution = std::uniform_real_distribution<float>(-maxSpeedVariation, maxSpeedVariation);
}

void particles::ParticleEmitter::setResistanceVariation(float resistanceVariation) {
	this->resistanceVariation = resistanceVariation;

	float maxResistanceVariation = particleResistance * resistanceVariation;
	resistanceDistribution = std::uniform_real_distribution<float>(-maxResistanceVariation, maxResistanceVariation);
}

void particles::ParticleEmitter::setEmitTimeVariation(float emitTimeVariation) {
	this->emitTimeVariation = emitTimeVariation;

	float maxEmitTimeVariation = particleEmitTimeSeconds * emitTimeVariation;
	emitTimeDistribution = std::uniform_real_distribution<float>(-maxEmitTimeVariation, maxEmitTimeVariation);
}

void particles::ParticleEmitter::setRotationSpeedVariation(float rotationSpeedVariation) {
	this->rotationSpeedVariation = rotationSpeedVariation;

	float maxRotationSpeedVariation = particleRotationsPerSecond * rotationSpeedVariation;
	rotationSpeedDistribution = std::uniform_real_distribution<float>(-maxRotationSpeedVariation, maxRotationSpeedVariation);
}

void particles::ParticleEmitter::setLifeTimeVariation(float lifeTimeVariation) {
	this->lifeTimeVariation = lifeTimeVariation;

	float maxLifeTimeVariation = particleLifeSeconds * lifeTimeVariation;
	lifeTimeDistribution = std::uniform_real_distribution<float>(-maxLifeTimeVariation, maxLifeTimeVariation);
}

void particles::ParticleEmitter::setDirectionVariation(float directionVariation) {
	this->directionVariation = directionVariation;

	float maxDirectionVariation = particleDirection * directionVariation;
	directionDistribution = std::uniform_real_distribution<float>(-maxDirectionVariation, maxDirectionVariation);
}

void particles::ParticleEmitter::setSizeVariation(float sizeVariation) {
	this->sizeVariation = sizeVariation;
	sizeDistribution = std::uniform_real_distribution<float>(1.F - sizeVariation, 1.F + sizeVariation);
}

void particles::ParticleEmitter::recalculateVertices() { //Pushes all of the vertices from all of the particles into a single vertex buffer.
	emitterVertexCount = 0U;

	if (!particles.empty()) {
		std::unique_ptr<sf::Vertex[]> vertices = std::make_unique<sf::Vertex[]>(4U);
		sf::Vertex * particleVerticesPointer = emitterParticleVertices.get();

		for (Particle & particle : particles) {
			for (unsigned int i = 0U; i < 4U; ++i) {
				vertices[i] = particle.vertices.get()[i];
				vertices[i].position += particle.location;
			}

			particleVerticesPointer[emitterVertexCount] = vertices[0U];
			particleVerticesPointer[emitterVertexCount + 1U] = vertices[1U];
			particleVerticesPointer[emitterVertexCount + 2U] = vertices[3U];
			particleVerticesPointer[emitterVertexCount + 3U] = vertices[1U];
			particleVerticesPointer[emitterVertexCount + 4U] = vertices[2U];
			particleVerticesPointer[emitterVertexCount + 5U] = vertices[3U];

			emitterVertexCount += 6U;
		}
	}
}

particles::ParticleManager::ParticleManager() {
	particleEmitters = std::list<ParticleEmitter>();
	particleVertexBuffer = sf::VertexBuffer(sf::PrimitiveType::Triangles, sf::VertexBuffer::Stream);
	managerRenderStates = engine::CombinedRenderStates();
}

void particles::ParticleManager::update(float deltaSeconds) { //Updates each of the emitter's the particle manages.
	if (!particleEmitters.empty()) {
		unsigned int vertexCount = 0U;
		bool vertexCountChanged = false;

		for (std::list<ParticleEmitter>::iterator emitterIterator = particleEmitters.begin(); emitterIterator != particleEmitters.end(); ) {
			emitterIterator->update(deltaSeconds);
			vertexCount += emitterIterator->getEmitterVertexCount();
			vertexCountChanged |= emitterIterator->getEmitterVertexCountChanged();

			if (emitterIterator->getEmitterFinished()) { //Remove the emitter if its lifetime is over.
				emitterIterator = particleEmitters.erase(emitterIterator);
			}

			else {
				++emitterIterator;
			}
		}

		if (vertexCountChanged && vertexCount != particleVertexBuffer.getVertexCount()) {
			particleVertexBuffer.create(vertexCount);
		}

		recollectVertices();
	}
}

void particles::ParticleManager::zoomManager(float scaleFactor) {
	if (!particleEmitters.empty()) {
		unsigned int i = 0U;

		for (ParticleEmitter & particleEmitter : particleEmitters) {
			particleEmitter.zoomEmitter(scaleFactor);
			particleVertexBuffer.update(particleEmitter.getEmitterParticleVertices(), particleEmitter.getEmitterVertexCount(), i);

			i += particleEmitter.getEmitterVertexCount();
		}
	}
}

particles::ParticleEmitter & particles::ParticleManager::addEmitter(bool atBack) {
	if (atBack) {
		particleEmitters.emplace_back();
		return *particleEmitters.rbegin();
	}

	else {
		particleEmitters.emplace_front();
		return *particleEmitters.begin();
	}
}

void particles::ParticleManager::clearEmitters() {
	particleEmitters.clear();
	particleVertexBuffer.create(0U);
}

void particles::ParticleManager::recollectVertices() {
	if (!particleEmitters.empty()) {
		unsigned int i = 0U;

		for (ParticleEmitter & particleEmitter : particleEmitters) {
			particleVertexBuffer.update(particleEmitter.getEmitterParticleVertices(), particleEmitter.getEmitterVertexCount(), i);
			i += particleEmitter.getEmitterVertexCount();
		}
	}
}
