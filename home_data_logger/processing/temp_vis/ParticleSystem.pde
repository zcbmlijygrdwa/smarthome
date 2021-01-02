class ParticleSystem {
  ArrayList<Particle> particles;

  PShape particleShape;

  ParticleSystem(int n) {
    particles = new ArrayList<Particle>();
    particleShape = createShape(PShape.GROUP);

    for (int i = 0; i < n; i++) {
      Particle p = new Particle();
      particles.add(p);
      particleShape.addChild(p.getShape());
    }
  }

  void update() {
    for (Particle p : particles) {
      p.update();
    }
  }

  void setEmitter(float x, float y, int value) {
    for (Particle p : particles) {
      if (p.isDead()) {
        p.rebirth(x, y);
        p.setColor(value, 255-value,255);
      }
    }
  }

  void display() {

    shape(particleShape);
  }
}
