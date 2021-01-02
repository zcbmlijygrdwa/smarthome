  class Particle {
  
    PVector velocity;
    float lifespan = 255;
    
    PShape part;
    float partSize;
    
    int r = 255;
    int g = 255;
    int b = 255;

    PVector gravity = new PVector(0,0.1);
  
  
    Particle() {
      partSize = random(10,60);
      part = createShape();
      part.beginShape(QUAD);
      part.noStroke();
      part.texture(sprite);
      part.normal(0, 0, 1);
      part.vertex(-partSize/2, -partSize/2, 0, 0);
      part.vertex(+partSize/2, -partSize/2, sprite.width, 0);
      part.vertex(+partSize/2, +partSize/2, sprite.width, sprite.height);
      part.vertex(-partSize/2, +partSize/2, 0, sprite.height);
      part.endShape();
      
      rebirth(width/2,height/2);
      lifespan = random(255);
    }
  
    PShape getShape() {
      return part;
    }
    
    void rebirth(float x, float y) {
      float a = random(TWO_PI);
      float speed = random(0.5,4);
      velocity = new PVector(cos(a), sin(a));
      velocity.mult(speed);
      lifespan = 255;   
      part.resetMatrix();
      part.translate(x, y); 
    }
    
    boolean isDead() {
      if (lifespan < 0) {
       return true;
      } else {
       return false;
      } 
    }
    
    void setColor(int r_i, int g_i, int b_i){
      r = r_i;
      g = g_i;
      b = b_i;
    }
  
    public void update() {
      lifespan = lifespan - 1;
      velocity.add(gravity);
      
      part.setTint(color(r,g,b,lifespan));
      part.translate(velocity.x, velocity.y);
    }
  }
