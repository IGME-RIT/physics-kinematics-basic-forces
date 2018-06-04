#include "GameObject.h"



void GameObject::update(float dt, glm::mat4 PV)
{
	//Summing all forces acting on the object
	addForces();

	//Calculating position based of kinematics equations
	eulerIntegration(dt);

	//translating object
	translation = glm::translate(glm::vec3(position));

	//Calculating model matrix and then updating the objects MVP
	calcTransform();
	MVP = PV * transformation;

	//Zeroing force and acceleration
	totalForce = acceleration = glm::vec3();


}

void GameObject::addForces()
{
	totalForce += GRAVITY * mass; //Gravitational force


	if (position.y < -1)
	{
		position.y=-1;

		if (velocity.y < 0)
			totalForce += velocity*-100.f; // Bounce force
	}

	acceleration = totalForce / mass; //Force=m * a so a = F/m
}

//Calculates Transformation matrix -> T * R * S
void GameObject::calcTransform()
{
	transformation = translation * rotation * scale;
	
}

void GameObject::eulerIntegration(float dt)
{
	//Kinematic equation x= v*dt+ 0.5(a *dt^2)
	position += velocity * dt + 0.5f*(acceleration * dt*dt);

	//Updating velocity
	velocity += acceleration * dt;

}


//Sets up the vertices in the vertex buffer
void GameObject::setupCircle( float r , float n)
{

	float radius = r;


	VertexFormat center(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	std::vector<VertexFormat> vertices;

	float theta = 360.0f / n;

	//Circle vertex generation
	//In this example we are not implementing the proper the code for indices. We are just going to produce redundant information in the buffer.
	//since we are only having a small number of objects on the screen currently, this redundancy should not matter.
	for (int i = 0; i < n; i++)
	{
		//In every iteration, the center, the point at angle theta and at angle (theta+delta) are fed into the buffer.
		vertices.push_back(center);
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(glm::radians(i*theta)), radius * sin(glm::radians(i*theta)), 0.0f), glm::vec4(0.7f, 0.20f, 0.0f, 1.0f)));
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(glm::radians((i + 1)*theta)), radius * sin(glm::radians((i + 1)*theta)), 0.0f), glm::vec4(0.7f, 0.20f, 0.0f, 1.0f)));
	}

	base.initBuffer(n * 3, &vertices[0]);
}


//Initalizing values
GameObject::GameObject(glm::mat4 mvp)
{
	velocity = totalForce = acceleration = position = glm::vec3();

	translation=rotation=scale=transformation= glm::mat4();

	mass = 1.0f;

	MVP = mvp;
}


GameObject::~GameObject()
{
}
