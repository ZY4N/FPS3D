#include <graphics/lineRenderer.hpp>

void lineRenderer::render(
	const std::vector<renderable>& renderables,
	const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix
) {
	lines->bind();
	lines->set<"projectionMat">(projectionMatrix);
	lines->set<"viewMat">(viewMatrix);
	lines->set<"lineColor">(lineColor);
	
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

		lines->bind();
		lines->set<"modelMat">(item.transform);
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);			
		glDrawElements(GL_TRIANGLES, item.numIndices, GL_UNSIGNED_SHORT, 0);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		lines->unbind();
		
		glBindVertexArray(0);
	}
}