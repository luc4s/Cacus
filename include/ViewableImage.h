class ViewableImage {
  // Create image + view
  ViewableImage() {

  }

  // Only create the view
  ViewableImage(VkImage image) {

  }

private:
  VkImageView imageView;
  VkImage image;
};
