#include <graphics/defaultRenderer.hpp>

void defaultRenderer::render(
	const std::vector<renderable>& renderables,
	const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix
) {
	main->bind();
	main->set<"projectionMat">(projectionMatrix);
	main->set<"viewMat">(viewMatrix);

	for (const renderable& item : renderables) {
		main->bind();
		glActiveTexture(GL_TEXTURE0);
		
		main->set<"modelMat">(item.transform);

		glBindVertexArray(item.vba);

		main->template set<"colorMerge">((float)!item.myTexture);

		if (item.myColor) {
			main->set<"meshColor">(item.myColor->c);
		}
		if (item.myTexture) {
			glBindTexture(GL_TEXTURE_2D, item.myTexture->textureID);
		}

		glDrawElements(GL_TRIANGLES, item.numIndices, GL_UNSIGNED_SHORT, 0);
		glActiveTexture(0);
		glBindVertexArray(0);
	}
}